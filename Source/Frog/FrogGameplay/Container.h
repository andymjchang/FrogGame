// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class UContainerComponent;
class UInteractableWidgetComponent;

UCLASS()
class FROG_API AContainer : public AInteractable
{
	GENERATED_BODY()

public:
	AContainer();
	virtual void BeginPlay() override;

	FORCEINLINE UContainerComponent* GetContainerComponent() { return ContainerComponent; }
	
protected:
	UFUNCTION()
	virtual void HandleRemovedFromInventory(AInteractable* Interactable);
	UFUNCTION()
	virtual void HandleAddedToInventory(AInteractable* Interactable);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UContainerComponent> ContainerComponent;
};
