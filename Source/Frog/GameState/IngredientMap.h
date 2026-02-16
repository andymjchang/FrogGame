// IngredientMap.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "IngredientMap.generated.h"

class AInteractable;
class UInteractableData;

// Wrapper struct that serves as the map key
USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer Tags;

	// Equality operator required for TMap
	bool operator==(const FRecipe& Other) const
	{
		return Tags.HasAllExact(Other.Tags) && 
			   Other.Tags.HasAllExact(Tags);
	}

	friend uint32 GetTypeHash(const FRecipe& Behavior)
	{
		uint32 Hash = 0;
		for (const FGameplayTag& Tag : Behavior.Tags)
		{
			Hash = HashCombine(Hash, GetTypeHash(Tag));
		}
		return Hash;
	}
};

USTRUCT(BlueprintType)
struct FRecipeResult
{
	GENERATED_BODY()
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AInteractable> InteractableClass;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInteractableData* InteractableData;

	FRecipeResult()
		: InteractableClass(nullptr)
		, InteractableData(nullptr)
	{}
};

// The main mapping Data Asset
UCLASS(BlueprintType)
class FROG_API UIngredientMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mapping")
	TMap<FRecipe, FRecipeResult> BehaviorToInteractableMap;

	UFUNCTION(BlueprintCallable, Category = "Mapping")
	const FRecipeResult& LookupInteractableClassByTagContainer(const FGameplayTagContainer& InTags) const;
};