// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"

ARoomActor::ARoomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a root component 
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(Root);

	// Floor component
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FloorMesh"));
	FloorMesh->SetupAttachment(RootComponent);

	TallWallArray.Init(false, NUM_ROOM_DIRECTIONS);
	DoorArray.Init(EDoorTypes::None, NUM_ROOM_DIRECTIONS);
	
	InitializeComponentsAroundHexagon();
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
		if (TallWallArray[i] == true)
		{
			if (UStaticMeshComponent* Wall = WallMeshArray[i])
			{
				Wall->SetStaticMesh(WallTallMesh);	
			}
		}
	}
}

void ARoomActor::InitializeComponentsAroundHexagon()
{
	WallMeshArray.Reserve(NUM_ROOM_DIRECTIONS);
	DoorNodeArray.Reserve(NUM_ROOM_DIRECTIONS);
	
	for (int32 i = 0; i < NUM_ROOM_DIRECTIONS; i++)
	{
		// Create Wall Static mesh //
		FString ComponentName = FString::Printf(TEXT("WallMesh%d"), i + 1);
		UStaticMeshComponent* NewWall = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		NewWall->SetupAttachment(RootComponent);
        
		// 60 degrees in radians = PI / 3
		float Angle = i * (PI / 3.0f); 

		FVector HexLocation;
		HexLocation.X = WallMidpointLength * FMath::Cos(Angle);
		HexLocation.Y = WallMidpointLength * FMath::Sin(Angle);
		HexLocation.Z = 0.f;
		NewWall->SetRelativeLocation(HexLocation);
        
		FRotator HexRotation = FRotator(0.0f, i * 60.f, 0.0f);
		NewWall->SetRelativeRotation(HexRotation);

		WallMeshArray.Add(NewWall);
		
		// Create Door Node //
		FString NodeName = FString::Printf(TEXT("DoorNode%d"), i + 1);
		USceneComponent* NewDoor = CreateDefaultSubobject<USceneComponent>(*NodeName);
		NewDoor->SetupAttachment(RootComponent);
		
		FVector DoorLocation = HexLocation;
		DoorLocation.Z = FloorHeight;
		NewDoor->SetRelativeLocation(DoorLocation);
		NewDoor->SetRelativeRotation(HexRotation);
		
		DoorNodeArray.Add(NewDoor);
	}
}

