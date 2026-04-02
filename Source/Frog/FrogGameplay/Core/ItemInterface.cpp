// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInterface.h"

#include "GameplayTagContainer.h"
#include "ItemData.h"


bool IItemInterface::HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const
{
	if (AcceptedTags.IsEmpty()) return false;
	
	UItemData* Data = GetData();
	if (!IsValid(Data)) return false;

	for (const FGameplayTag& OwnedTag : Data->GetOwnedTags())
	{
		if (OwnedTag.MatchesAny(AcceptedTags))
		{
			return true;
		}
	}

	return false;
}

void IItemInterface::EventAddedToAnotherInventory()
{
}
