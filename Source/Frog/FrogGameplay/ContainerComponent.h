// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "ItemData.h"
#include "Components/SceneComponent.h"
#include "ContainerComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AItem*, Item);

class UInteractableWidgetComponent;
class AContainer;
class AItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UContainerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UContainerComponent();
	void Initialize(UItemData* InData, UInteractableWidgetComponent* InWidgetComponent);

	void ClearInventory();
	void RemoveNullsFromInventory();
	bool TryAddToInventory(AItem* InteractableToAdd);
	bool TryAddContainerContentsToInventory(AContainer* ContainerToAdd);
	bool TryRemoveFromInventory(AItem* InteractableToRemove);

	void SetShowInventoryWidget(bool bShow);

	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;

	FORCEINLINE bool IsEmpty() const { return Inventory.Num() <= 0; }
	FORCEINLINE bool IsFull() const { return Data.IsValid() ? Inventory.Num() >= Data->GetMaxCapacity() : true; }
	FORCEINLINE AItem* GetFirstItem() const { return Inventory.Num() > 0 ? Inventory[0].Get() : nullptr; }
	FORCEINLINE const TArray<TObjectPtr<AItem>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	
protected:
	void UpdateInventoryWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool bShowInventoryWidget = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AItem>> Inventory;
	
	TWeakObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	TWeakObjectPtr<UItemData> Data;
};
