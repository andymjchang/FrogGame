// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemInterface.generated.h"

struct FGameplayTagContainer;
class IInteractableInterface;
class UItemData;
// This class does not need to be modified.
UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROG_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual UItemData* GetData() const = 0;
	virtual TScriptInterface<IItemInterface> GetOfferedInteractable() const = 0;
	
	virtual bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	
	virtual void EnableHitbox() = 0;
	virtual	void DisableHitbox() = 0;
	
	virtual void EventAddedToAnotherInventory();
};
