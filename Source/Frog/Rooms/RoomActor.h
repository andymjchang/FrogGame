// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypes.h"
#include "RoomActor.generated.h"

UCLASS()
class FROG_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();
	
	void SetWallArray(const TArray<bool>& InputArray);
	void SetDoorArray(const TArray<EDoorTypes>& InputArray);
	
protected: // Components
	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMeshComponent*> WallMeshes;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> FloorMesh;

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
	
private:
	float WallMidpointLength = 833.5f;
	
private:
	void CreateWallComponents();
};