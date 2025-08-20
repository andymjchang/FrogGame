// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "AbilityIcon.generated.h"

class UProgressBar;
class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class UI_API UAbilityIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeWidget(UAbilitySystemComponent* InputAbilitySystem, UGameplayAbility* InputAbility);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;
	
protected:
	UPROPERTY()
	UAbilitySystemComponent* TrackedAbilitySystem;

	UPROPERTY()
	TWeakObjectPtr<UGameplayAbility> TrackedAbility;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};