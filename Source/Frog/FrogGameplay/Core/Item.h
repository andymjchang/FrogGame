// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UItemData;
class UBoxComponent;

UCLASS()
class FROG_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AItem();
	
	virtual void StartInteract() override;
	virtual void StopInteract() override;
	virtual void StartHighlight() override;
	virtual void StopHighlight() override;
	virtual FVector GetInteractableLocation() override;
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;

	void EnableInteractable();
	void DisableInteractable();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UMaterialInterface> OverlayMaterial;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AItem> OfferedInteractable;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

public:
	FORCEINLINE UItemData* GetData() const { return Data; }
	FORCEINLINE AItem* GetOfferedInteractable() const { return OfferedInteractable.Get(); };
};
