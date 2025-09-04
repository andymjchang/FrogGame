// FrogTargetedProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "FrogTargetedProjectile.generated.h"

class AProjectile;

UCLASS()
class FROG_API UFrogTargetedProjectile : public UFrogGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	int32 NumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	float DelayBetweenShots = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	float SpreadAngleDegrees = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	TSubclassOf<AActor> TargetComponentClass;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Ability")
	TObjectPtr<USceneComponent> TargetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	float TargetComponentLifeSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Ability")
	FGameplayTag TargetEnemiesStateTag;

private:
	UFUNCTION()
	void FireProjectile();
	
	FTimerHandle ProjectileTimerHandle;

	int32 ProjectilesFired = 0;
};