// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FA_Interact.generated.h"

class AFrogCharacter;
class IItemInterface;

UCLASS()
class FROG_API UFA_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFA_Interact();
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void PickupInteractable(const AFrogCharacter* Frog, IItemInterface* Item);

protected:
	UFUNCTION()
	void HandleInputReleased(float TimeHeld);
};
