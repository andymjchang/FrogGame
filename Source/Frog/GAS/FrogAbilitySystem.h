// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FrogAbilitySystem.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None = 0 UMETA(Hidden),
	Interact = 1,
	Work = 2
};
/**
 * 
 */
UCLASS()
class FROG_API UFrogAbilitySystem : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
};
