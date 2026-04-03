// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class UContainerComponent;
class UInteractableWidgetComponent;

UCLASS()
class FROG_API AContainer : public AItem
{
	GENERATED_BODY()

public:
	AContainer();

	UContainerComponent* GetContainerComponent() { return ContainerComponent; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UFUNCTION()
    virtual void HandleAddedToInventory(const TScriptInterface<IItemInterface>& Interactable);
	UFUNCTION()
	virtual void HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Container")
	TObjectPtr<UContainerComponent> ContainerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
};
