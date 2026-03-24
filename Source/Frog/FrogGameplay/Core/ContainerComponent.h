// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "ItemData.h"
#include "Components/SceneComponent.h"
#include "ContainerComponent.generated.h"

class UInteractableWidgetComponent;
class AContainer;
class AItem;
class UMeshComponent;
class IItemInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChanged, const TScriptInterface<IItemInterface>&, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UContainerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UContainerComponent();
	void Initialize(UItemData* InData, UInteractableWidgetComponent* InWidgetComponent, UMeshComponent* InMesh);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ClearInventory();
	void RemoveNullsFromInventory();
	bool TryAddToInventory(const TScriptInterface<IItemInterface>& InteractableToAdd, UContainerComponent* SourceContainerComp = nullptr);
	bool TryAddContainerContentsToInventory(UContainerComponent* SourceContainerComp);
	bool TryRemoveFromInventory(TScriptInterface<IItemInterface> InteractableToRemove);

	void SetShowInventoryWidget(bool bShow);

	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;

	bool IsEmpty() const { return Inventory.Num() <= 0; }
	bool IsFull() const { return Data.IsValid() ? Inventory.Num() >= Data->GetMaxCapacity() : true; }
	TScriptInterface<IItemInterface> GetFirstItem() const { return Inventory.Num() > 0 ? Inventory[0] : nullptr; }
	TArray<TScriptInterface<IItemInterface>> GetInventory() const { return Inventory; }
	int GetInventorySize() const { return Inventory.Num(); }
	bool GetAllowAdd() const { return bAllowAdd; }
	void SetAllowAdd(const bool InAllowAdd) { bAllowAdd = InAllowAdd; }
	bool GetAllowRemove() const { return bAllowRemove; }
	void SetAllowRemove(const bool InAllowRemove) { bAllowRemove = InAllowRemove; }
	
protected:
	void UpdateInventoryWidget();

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_IsInventoryWidgetVisible();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsInventoryWidgetVisible)
	bool bIsInventoryWidgetVisible = true;
	
	UPROPERTY(Transient, Replicated)
	bool bAllowAdd = true;
	
	UPROPERTY(Replicated)
	bool bAllowRemove = true;

	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	TArray<TScriptInterface<IItemInterface>> Inventory;
	
	TWeakObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	TWeakObjectPtr<UItemData> Data;
	TWeakObjectPtr<UMeshComponent> Mesh;
};
