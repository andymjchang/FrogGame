// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypes.h"
#include "RoomActor.generated.h"

class UDoorComponent;

UCLASS()
class FROG_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();
	
	void SetTallWallArray(const TArray<bool>& InputArray);
	void SetDoorArray(const TArray<EDoorTypes>& InputArray);
	void SetMeshes();

protected: // Data
	TArray<bool> TallWallArray;
	TArray<EDoorTypes> DoorArray;
	
protected: // Loading assets
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> WallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> WallDoorMesh;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> WallTallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> WallDoorTallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> BlockedDoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UMaterialInterface> WallMaterial;
	
protected: // Components
	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMeshComponent*> WallMeshArray;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<USceneComponent*> DoorNodeArray;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> FloorMesh;
	
private:
	static constexpr float WallMidpointLength = 833.5f;
	static constexpr float FloorHeight = 10.f;
	
private:
	void InitializeComponentsAroundHexagon();
};