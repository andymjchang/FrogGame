// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientMap.h"

const FRecipeResult& UIngredientMap::LookupInteractableClassByTagContainer(const FGameplayTagContainer& InTags) const
{
	FRecipe Behavior;
	Behavior.Tags = InTags;
    
	if (const FRecipeResult* FoundResult = BehaviorToInteractableMap.Find(Behavior))
	{
		return *FoundResult;
	}

	static const FRecipeResult EmptyResult;
	return EmptyResult;
}
