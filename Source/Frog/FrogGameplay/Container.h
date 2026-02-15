// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class UInteractableWidgetComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AInteractable*, Item);

UCLASS()
class FROG_API AContainer : public AInteractable
{
	GENERATED_BODY()

public:
	AContainer();
	virtual void PostInitializeComponents() override;

public:
	void ClearInventory();
	
	bool TryAddToInventory(AInteractable* InteractableToAdd);
	bool TryAddContainerContentsToInventory(AContainer* ContainerToAdd);
	bool TryRemoveFromInventory(AInteractable* InteractableToRemove);
	
	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;
protected:
	void UpdateInventoryWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInteractable>> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
public:
	FORCEINLINE const TArray<TObjectPtr<AInteractable>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
};
