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

	FORCEINLINE UContainerComponent* GetContainerComponent() { return ContainerComponent; }
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
    virtual void HandleAddedToInventory(AItem* Interactable);
	UFUNCTION()
	virtual void HandleRemovedFromInventory(AItem* Interactable);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Container")
	TObjectPtr<UContainerComponent> ContainerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;
};
