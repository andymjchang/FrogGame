// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "FrogHUD.generated.h"

class UAbilityIcon;

UCLASS()
class UI_API UFrogHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AssignAbilityToUISlot(UGameplayAbility* InputAbility, UAbilitySystemComponent* InputAbilitySystem) const;
	
	UPROPERTY(meta = (BindWidget))
	UAbilityIcon* AbilityIconPrimary;
	UPROPERTY(meta = (BindWidget))
	UAbilityIcon* AbilityIconSecondary;
	UPROPERTY(meta = (BindWidget))
	UAbilityIcon* AbilityIconGrapple;
	UPROPERTY(meta = (BindWidget))
	UAbilityIcon* AbilityIconDive;
	UPROPERTY(meta = (BindWidget))
	UAbilityIcon* AbilityIconUtility;
};
