// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

struct FGameplayTagContainer;
class UItemData;

UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  
 */
class FROG_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void StartInteract();
	virtual void StopInteract();

	virtual void StartHighlight(UMaterialInterface* InHighlightMaterial);
	virtual void StopHighlight();
	
	virtual FVector GetInteractableLocation() const = 0;
	virtual UMeshComponent* GetInteractableMesh() const = 0;
};

