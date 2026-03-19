#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Dash.generated.h"

class UCurveFloat;

UCLASS()
class FROG_API UDash : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashStrength = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	UCurveFloat* DashStrengthOverTime;

	UFUNCTION()
	void OnDashCompleted();
};