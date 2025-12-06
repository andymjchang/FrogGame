// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FrogAbility.generated.h"

/**
 * CooldownDuration and CooldownTags allow us to use one GE BP for cooldowns,
 * with different cooldown tags set in each GA BP.
 */
UCLASS()
class FROG_API UFrogAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFrogAbility();
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                          FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	FVector GetCrosshairLocation(bool bGetClosestTarget) const;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION()
	void OnCooldownTagChanged(const FGameplayTag Tag, int32 NewCount);
	void AttemptReactivation();

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	bool bEndAbilityOnInputReleased;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	bool bRecastIfHeld;

	bool bIsHeld;

private:
	FDelegateHandle CooldownEventHandle;
	FTimerHandle ReactivationTimerHandle;

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
};