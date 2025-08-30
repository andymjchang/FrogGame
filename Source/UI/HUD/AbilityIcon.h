// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "AbilityIcon.generated.h"

class UProgressBar;
class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class UI_API UAbilityIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void TrackAbility(UAbilitySystemComponent* InputAbilitySystem, FGameplayAbilitySpecHandle InputAbilityHandle);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag AbilityTag;
	
protected:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> TrackedAbilitySystem;

	FGameplayAbilitySpecHandle TrackedAbilityHandle;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};