#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "IngredientMap.generated.h"

// Forward declaration (assuming UInteractableData is defined elsewhere)
class UInteractableData;

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
	TMap<FIngredientBehavior, UInteractableData*> BehaviorToInteractableMap;

	UFUNCTION(BlueprintCallable, Category = "Mapping")
	UInteractableData* GetInteractableByBehavior(const FGameplayTagContainer& Tags) const
	{
		FIngredientBehavior Behavior;
		Behavior.Tags = Tags;
        
		if (UInteractableData* const* FoundData = BehaviorToInteractableMap.Find(Behavior))
		{
			return *FoundData;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Mapping")
	UInteractableData* GetInteractableByBehaviorStruct(const FIngredientBehavior& Behavior) const
	{
		if (UInteractableData* const* FoundData = BehaviorToInteractableMap.Find(Behavior))
		{
			return *FoundData;
		}
		return nullptr;
	}
};