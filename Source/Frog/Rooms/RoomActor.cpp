// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"

#include "Door.h"
#include "RoomDefinition.h"

ARoomActor::ARoomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a root component 
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(Root);

	// Floor component
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Floor"));
	Floor->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorAsset(TEXT("/Game/Rooms/SM_Floor"));
	if (FloorAsset.Succeeded())
	{
		Floor->SetStaticMesh(FloorAsset.Object);
	}
	
	WallTypeArray.Init(false, NUM_ROOM_SIDES);
	DoorTypeArray.Init(EDoorTypes::None, NUM_ROOM_SIDES);
	
	InitializeComponentsAroundHexagon();
}

void ARoomActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    
	if (IsValid(RoomDefinition))
	{
		SetDoorTypeArray(RoomDefinition->DoorArray);
		RegenerateRoom();
	}
}

void ARoomActor::SetDoorTypeArray(const TArray<EDoorTypes>& InArray)
{
	DoorTypeArray = InArray;
}

void ARoomActor::SetWallTypeArray(const TArray<bool>& InArray)
{
	WallTypeArray = InArray;
}

void ARoomActor::RegenerateRoom()
{
	if (DoorTypeArray.Num() != DoorArray.Num()) return;
	if (WallTypeArray.Num() != WallArray.Num()) return;
	if (!DoorBlueprint) return;

	
	for (int i = 0; i < DoorArray.Num(); i++)
	{
		const bool bHasDoor = (DoorTypeArray[i] != EDoorTypes::None);
		const bool bIsTallWall = WallTypeArray[i];

#if WITH_EDITOR
		// // Create or destroy door object
		// if (UChildActorComponent* DoorComponent = DoorArray[i])
		// {
		// 	TSubclassOf<AActor> TargetClass{!bHasDoor ? nullptr : DoorBlueprint};
		// 	if (DoorComponent->GetChildActorClass() != TargetClass)
		// 	{
		// 		// Disconnect old delegate
		// 		if (ADoor* DoorActor = Cast<ADoor>(DoorComponent->GetChildActor()); IsValid(DoorActor))
		// 		{
		// 			DoorActor->OnProgressComplete.Unbind();
		// 		}
		//
		// 		DoorComponent->SetChildActorClass(TargetClass);
		//
		// 		// Setup new door
		// 		if (ADoor* DoorActor = Cast<ADoor>(DoorComponent->GetChildActor()); IsValid(DoorActor))
		// 		{
		// 			DoorActor->OnProgressComplete.BindDynamic(this, &ARoomActor::HandleDoorProgressComplete);
		// 			DoorActor->SetFacingDirection(static_cast<ERoomDirection>(i));
		// 		}
		// 	}
		// }
#endif

		// Set correct wall mesh
		if (UStaticMeshComponent* WallComponent = WallArray[i])
		{
			UStaticMesh* TargetMesh = nullptr;

			if (bIsTallWall)
			{
				TargetMesh = bHasDoor ? WallDoorTallMesh : WallTallMesh;
			}
			else
			{
				TargetMesh = bHasDoor ? WallDoorMesh : WallMesh;
			}

			if (WallComponent->GetStaticMesh() != TargetMesh)
			{
				WallComponent->SetStaticMesh(TargetMesh);
			}
		}
	}
}

void ARoomActor::HandleDoorProgressComplete(ERoomDirection FacingDirection, int32 UnlockPrice)
{
	OnRequestNewRoom.ExecuteIfBound(HexIndex, FacingDirection, UnlockPrice);
}

void ARoomActor::InitializeComponentsAroundHexagon()
{
	for (int32 i = NUM_ROOM_SIDES - 1; i >= 0; i--)
	{
		FString ComponentName = FString::Printf(TEXT("WallMesh%d"), i);
		UStaticMeshComponent* NewWall = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		NewWall->SetupAttachment(RootComponent);
        
		float Angle = i * (PI / 3.0f); 

		FVector HexLocation;
		HexLocation.X = WallMidpointLength * FMath::Cos(Angle);
		HexLocation.Y = WallMidpointLength * FMath::Sin(Angle);
		HexLocation.Z = 0.f;
		NewWall->SetRelativeLocation(HexLocation);
        
		FRotator HexRotation = FRotator(0.0f, i * 60.f, 0.0f);
		NewWall->SetRelativeRotation(HexRotation);

		static ConstructorHelpers::FObjectFinder<UStaticMesh> WallAsset(TEXT("/Game/Rooms/SM_Wall"));
		if (WallAsset.Succeeded())
		{
			NewWall->SetStaticMesh(WallAsset.Object);
		}
       
		WallArray.Insert(NewWall, 0);
       
		FString NodeName = FString::Printf(TEXT("DoorNode%d"), i);
		UChildActorComponent* NewDoor = CreateDefaultSubobject<UChildActorComponent>(*NodeName);
		NewDoor->SetupAttachment(RootComponent);
       
		FVector DoorLocation = HexLocation;
		DoorLocation.Z = FloorHeight;
		NewDoor->SetRelativeLocation(DoorLocation);
		NewDoor->SetRelativeRotation(HexRotation);
       
		DoorArray.Insert(NewDoor, 0);
	}
}

