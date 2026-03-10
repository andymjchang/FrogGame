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
	virtual FVector GetInteractableLocation() const override;
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;

	void EnableInteractable();
	void DisableInteractable();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;

	UPROPERTY(Transient)
	TObjectPtr<AItem> OfferedInteractable;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

public:
	FORCEINLINE UItemData* GetData() const { return Data; }
	FORCEINLINE AItem* GetOfferedInteractable() const { return OfferedInteractable.Get(); }
	FORCEINLINE virtual UStaticMeshComponent* GetInteractableMesh() const override { return InteractableMesh.Get(); }
};
