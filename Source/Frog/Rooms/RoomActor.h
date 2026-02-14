// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypes.h"
#include "RoomActor.generated.h"

class ADoor;
class UDoorComponent;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRequestNewRoom, FHexIndex, HexIndex, ERoomDirection, FacingDirection);

UCLASS()
class FROG_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	void SetDoorTypeArray(const TArray<EDoorTypes>& InArray);
	void SetWallTypeArray(const TArray<bool>& InArray);
	void RegenerateRoom();

	FORCEINLINE void SetHexIndex(const FHexIndex InHexIndex) { HexIndex = InHexIndex; }
	
	// Delegates
	FOnRequestNewRoom OnRequestNewRoom;

protected:
	UFUNCTION()
	void HandleDoorProgressComplete(ERoomDirection FacingDirection);
	
	void InitializeComponentsAroundHexagon();
	
protected: // Data
	TArray<bool> WallTypeArray;
	TArray<EDoorTypes> DoorTypeArray;
	FHexIndex HexIndex;
	
protected: // Loading assets
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<URoomDefinition> RoomDefinition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UStaticMesh> FloorMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UStaticMesh> WallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UStaticMesh> WallDoorMesh;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UStaticMesh> WallTallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UStaticMesh> WallDoorTallMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TSubclassOf<ADoor> DoorBlueprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UMaterialInterface> WallMaterial;

protected: // Components
	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMeshComponent*> WallArray;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<UChildActorComponent*> DoorArray;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Floor;
	
private:
	static constexpr float WallMidpointLength = 833.5f;
	static constexpr float FloorHeight = 10.f;
};