// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"

struct FGameplayAbilitySpecHandle;
enum class EAbilityInputID : uint8;
class UGameplayAbility;
class UAbilitySystemComponent;

USTRUCT()
struct FAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	EAbilityInputID InputKey;
};
/**
 * 
 */
UCLASS()
class FROG_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TArray<FAbilitySetItem> AbilitiesSetList;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};
