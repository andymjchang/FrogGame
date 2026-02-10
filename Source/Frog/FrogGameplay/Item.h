// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AItem;
class UInteractableWidgetComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AItem*, Item);

UCLASS()
class FROG_API AItem : public AInteractable
{
	GENERATED_BODY()

public:
	AItem();
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	void ClearInventory();
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	bool TryAddToInventory(AItem* InteractableToAdd);
	bool TryAddContainerToInventory(AItem* ContainerToAdd);
	bool TryRemoveFromInventory(AItem* InteractableToRemove);
	
	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;
protected:
	void UpdateInventoryWidget();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AItem> OfferedInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AItem>> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
public:
	FORCEINLINE const TArray<TObjectPtr<AItem>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE AItem* GetOfferedInteractable() const { return OfferedInteractable.Get(); };
	FORCEINLINE bool GetIsContainer() const { return Inventory.Num() > 0; }

};
