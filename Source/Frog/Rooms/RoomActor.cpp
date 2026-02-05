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
	
	TallWallArray.Init(false, NUM_ROOM_DIRECTIONS);
	DoorArray.Init(EDoorTypes::None, NUM_ROOM_DIRECTIONS);
	
	InitializeComponentsAroundHexagon();
}

void ARoomActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    
	if (!IsValid(RoomDefinition)) return;
	if (!DoorBlueprint) return;

	const int32 IterateCount = FMath::Min(DoorActorArray.Num(), RoomDefinition->DoorArray.Num());
	for (int32 i = 0; i < IterateCount; i++)
	{
		if (UChildActorComponent* DoorComp = DoorActorArray[i])
		{
			if (RoomDefinition->DoorArray[i] == EDoorTypes::Locked)
			{
				DoorComp->SetChildActorClass(DoorBlueprint);
			}
			else
			{
				DoorComp->SetChildActorClass(nullptr);
			}
		}
		if (UStaticMeshComponent* WallComponent = WallMeshArray[i])
		{
			if (RoomDefinition->DoorArray[i] == EDoorTypes::Locked)
			{
				WallComponent->SetStaticMesh(WallDoorMesh);
			}
			else
			{
				WallComponent->SetStaticMesh(WallMesh);
			}
		}
	}
}
void ARoomActor::SetTallWallArray(const TArray<bool>& InputArray)
{
	TallWallArray = InputArray;
}

void ARoomActor::SetDoorArray(const TArray<EDoorTypes>& InputArray)
{
	DoorArray = InputArray;
}

void ARoomActor::SetMeshes()
{
	for (int32 i = 0; i < 6; i++)
	{
		if (UStaticMeshComponent* Wall = WallMeshArray[i])
		{
			if (TallWallArray[i] == true)
			{
				Wall->SetStaticMesh(WallTallMesh);	
			}
			else
			{
				Wall->SetStaticMesh(WallMesh);
			}
		}
	}
}

void ARoomActor::InitializeComponentsAroundHexagon()
{
	WallMeshArray.Reserve(NUM_ROOM_DIRECTIONS);
	DoorActorArray.Reserve(NUM_ROOM_DIRECTIONS);
    
	for (int32 i = NUM_ROOM_DIRECTIONS - 1; i >= 0; i--)
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
       
		WallMeshArray.Insert(NewWall, 0);
       
		FString NodeName = FString::Printf(TEXT("DoorNode%d"), i);
		UChildActorComponent* NewDoor = CreateDefaultSubobject<UChildActorComponent>(*NodeName);
		NewDoor->SetupAttachment(RootComponent);
       
		FVector DoorLocation = HexLocation;
		DoorLocation.Z = FloorHeight;
		NewDoor->SetRelativeLocation(DoorLocation);
		NewDoor->SetRelativeRotation(HexRotation);
       
		DoorActorArray.Insert(NewDoor, 0);
	}
}

