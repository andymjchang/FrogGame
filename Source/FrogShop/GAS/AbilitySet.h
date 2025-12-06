// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrogAbilitySystem.h"
#include "UI/HUD/FrogHUD.h"
#include "AbilitySet.generated.h"

struct FGameplayAbilitySpecHandle;
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
	EAbilityInputID InputKey = EAbilityInputID::None;
};
/**
 * 
 */
UCLASS()
class FROGSHOP_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TArray<FAbilitySetItem> AbilitiesSetList;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* InputAbilitySystem, const UFrogHUD* FrogHUD) const;
};
