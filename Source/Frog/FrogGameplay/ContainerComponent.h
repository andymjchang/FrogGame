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
	void Initialize(UInteractableData* InData);

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
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	void UpdateInventoryWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInteractable>> Inventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
	UPROPERTY()
	TWeakObjectPtr<UInteractableData> Data;
};
