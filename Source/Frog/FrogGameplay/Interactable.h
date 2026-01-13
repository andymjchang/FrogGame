// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class AInteractable;
class UInteractableWidgetComponent;
class UInteractableData;
class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChanged, AInteractable*);

UCLASS()
class FROG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	
public:
	// Delegate 
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;
	
	void EnableInteractable();
	void DisableInteractable();
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	bool TryAddToInventory(AInteractable* InteractableToAdd);
	bool TryRemoveFromInventory(AInteractable* InteractableToRemove);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UInteractableData> Data;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractable> OfferedInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInteractable>> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
public:
	FORCEINLINE UInteractableData* GetData() const { return Data; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE AInteractable* GetOfferedInteractable() const { return OfferedInteractable.Get(); };

};
