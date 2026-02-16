// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "Components/SceneComponent.h"
#include "ContainerComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AInteractable*, Item);

class UInteractableWidgetComponent;
class AContainer;
class AInteractable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UContainerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UContainerComponent();
	virtual void BeginPlay() override;

	void ClearInventory();
	bool TryAddToInventory(AInteractable* InteractableToAdd);
	bool TryAddContainerContentsToInventory(AContainer* ContainerToAdd);
	bool TryRemoveFromInventory(AInteractable* InteractableToRemove);

	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;

	FORCEINLINE const TArray<TObjectPtr<AInteractable>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE USceneComponent* GetAttachPoint() const { return AttachPoint; }
	
protected:
	void UpdateInventoryWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInteractable>> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
	UPROPERTY()
	TWeakObjectPtr<UInteractableData> Data;
};
