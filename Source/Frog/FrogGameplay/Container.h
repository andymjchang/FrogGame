// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class AContainer;
class UInteractableWidgetComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInventoryItemChanged, class AContainer*, Item);

UCLASS()
class FROG_API AContainer : public AInteractable
{
	GENERATED_BODY()

public:
	AContainer();
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	void ClearInventory();
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	bool TryAddToInventory(AContainer* InteractableToAdd);
	bool TryAddContainerToInventory(AContainer* ContainerToAdd);
	bool TryRemoveFromInventory(AContainer* InteractableToRemove);
	
	// Delegates
	FOnInventoryItemChanged OnRemovedFromInventory;
	FOnInventoryItemChanged OnAddedToInventory;
protected:
	void UpdateInventoryWidget();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AContainer> OfferedInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AContainer>> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> AttachPoint;
	
public:
	FORCEINLINE const TArray<TObjectPtr<AContainer>>& GetInventory() const { return Inventory; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE AContainer* GetOfferedInteractable() const { return OfferedInteractable.Get(); };
	FORCEINLINE bool GetIsContainer() const { return Inventory.Num() > 0; }

};
