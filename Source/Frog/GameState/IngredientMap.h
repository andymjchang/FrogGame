// IngredientMap.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "IngredientMap.generated.h"

// Forward declaration
class AInteractable;

// Wrapper struct that serves as the map key
USTRUCT(BlueprintType)
struct FIngredientBehavior
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;

	// Equality operator required for TMap
	bool operator==(const FIngredientBehavior& Other) const
	{
		return Tags.HasAllExact(Other.Tags) && 
			   Other.Tags.HasAllExact(Tags);
	}

	friend uint32 GetTypeHash(const FIngredientBehavior& Behavior)
	{
		uint32 Hash = 0;
		for (const FGameplayTag& Tag : Behavior.Tags)
		{
			Hash = HashCombine(Hash, GetTypeHash(Tag));
		}
		return Hash;
	}
};

// The main mapping Data Asset
UCLASS(BlueprintType)
class FROG_API UIngredientMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mapping")
	TMap<FIngredientBehavior, TSubclassOf<AInteractable>> BehaviorToInteractableMap;

	UFUNCTION(BlueprintCallable, Category = "Mapping")
	TSubclassOf<AInteractable> GetInteractableClassByBehavior(const FGameplayTagContainer& InTags) const
	{
		FIngredientBehavior Behavior;
		Behavior.Tags = InTags;
        
		if (TSubclassOf<AInteractable> const* FoundClass = BehaviorToInteractableMap.Find(Behavior))
		{
			return *FoundClass;
		}
		return nullptr;
	}
};