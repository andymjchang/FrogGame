#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomDefinition.generated.h"

enum class EDoorTypes : uint8;
/**
 * Data asset to define the visual and gameplay properties of a room.
 */
UCLASS(BlueprintType)
class FROG_API URoomDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	URoomDefinition();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals", meta = (EditFixedSize))
	TArray<EDoorTypes> DoorArray;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> DoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UMaterialInterface> DoorMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> WindowMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UMaterialInterface> WindowMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UStaticMesh> BlockedDoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UMaterialInterface> BlockedDoorMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room Visuals")
	TObjectPtr<UMaterialInterface> FloorMaterial;
};
