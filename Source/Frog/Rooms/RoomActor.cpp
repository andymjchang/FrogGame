// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"

ARoomActor::ARoomActor()
{
	PrimaryActorTick.bCanEverTick = false;
    
	// Create a root component 
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(Root);
	
	CreateWallComponents();
	
	// Floor component
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FloorMesh"));
	FloorMesh->SetupAttachment(RootComponent);
}

void ARoomActor::SetWallArray(const TArray<bool>& InputArray)
{
	for (int32 i = 0; i < 6; i++)
	{
		if (InputArray[i] == true)
		{
			if (UStaticMeshComponent* Wall = WallMeshes[i])
			{
				Wall->SetStaticMesh(WallTallMesh);	
			}
		}
	}
}

void ARoomActor::SetDoorArray(const TArray<EDoorTypes>& InputArray)
{
	
}

void ARoomActor::CreateWallComponents()
{
	WallMeshes.Reserve(6);
	
	for (int32 i = 0; i < 6; i++)
	{
		FString ComponentName = FString::Printf(TEXT("WallMesh%d"), i + 1);
		UStaticMeshComponent* NewWall = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		NewWall->SetupAttachment(RootComponent);
        
		// 60 degrees in radians = PI / 3
		float Angle = i * (PI / 3.0f); 

		// Set Location
		FVector HexLocation;
		HexLocation.X = WallMidpointLength * FMath::Cos(Angle);
		HexLocation.Y = WallMidpointLength * FMath::Sin(Angle);
		HexLocation.Z = 0.f;
		
		NewWall->SetRelativeLocation(HexLocation);
        
		// Set Rotation
		FRotator HexRotation = FRotator(0.0f, i * 60.f, 0.0f);
		NewWall->SetRelativeRotation(HexRotation);

		WallMeshes.Add(NewWall);
	}
}

