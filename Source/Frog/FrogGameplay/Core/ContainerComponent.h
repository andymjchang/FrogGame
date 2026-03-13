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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChanged, AItem*, Item);

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
	bool TryAddToInventory(AItem* InteractableToAdd, UContainerComponent* SourceContainerComp = nullptr);
	bool TryAddContainerContentsToInventory(UContainerComponent* SourceContainerComp);
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
	FORCEINLINE bool GetAllowAdd() const { return bAllowAdd; }
	FORCEINLINE void SetAllowAdd(const bool InAllowAdd) { bAllowAdd = InAllowAdd; }
	FORCEINLINE bool GetAllowRemove() const { return bAllowRemove; }
	FORCEINLINE void SetAllowRemove(const bool InAllowRemove) { bAllowRemove = InAllowRemove; }
	
protected:
	void UpdateInventoryWidget();

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_IsInventoryWidgetVisible();

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsInventoryWidgetVisible)
	bool bIsInventoryWidgetVisible = true;
	
	UPROPERTY(Transient, Replicated)
	bool bAllowAdd = true;
	
	UPROPERTY( Replicated)
	bool bAllowRemove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Inventory)
	TArray<TObjectPtr<AItem>> Inventory;
	
	TWeakObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	TWeakObjectPtr<UItemData> Data;
	TWeakObjectPtr<UMeshComponent> Mesh;
};
