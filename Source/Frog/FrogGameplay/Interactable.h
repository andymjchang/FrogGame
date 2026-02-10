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

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AInteractable*, Item);

UCLASS()
class FROG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	void EnableInteractable();
	void DisableInteractable(); // TODO: check if enable/disable are redundant since items are always on a counter, inside container, or inside station
	void ClearInventory();
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	bool TryAddToInventory(AInteractable* InteractableToAdd);
	bool TryAddContainerToInventory(AInteractable* ContainerToAdd);
	bool TryRemoveFromInventory(AInteractable* InteractableToRemove);
	
	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;
protected:
	void UpdateInventoryWidget();

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
	FORCEINLINE const TArray<TObjectPtr<AInteractable>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE AInteractable* GetOfferedInteractable() const { return OfferedInteractable.Get(); };
	FORCEINLINE bool GetIsContainer() const { return Inventory.Num() > 0; }

};
