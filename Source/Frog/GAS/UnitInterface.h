// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitInterface.generated.h"

class UAbilitySystemComponent;

UINTERFACE()
class UUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROG_API IUnitInterface
{
	GENERATED_BODY()
	
public:
	virtual void HandleDeath() = 0;
	virtual void SetHealth(const float NewHealth) = 0;
	virtual void SetMaxHealth(const float NewMaxHealth) = 0;
};
