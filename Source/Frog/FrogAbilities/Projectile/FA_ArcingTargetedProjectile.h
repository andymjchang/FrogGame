// FrogTargetedProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "FA_Projectile.h"
#include "Abilities/GameplayAbility.h"
#include "FA_ArcingTargetedProjectile.generated.h"

class AProjectile;

UCLASS()
class FROG_API UFA_ArcingTargetedProjectile : public UFA_Projectile
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	int32 NumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	float DelayBetweenShots = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	float MinSpreadAngleDegrees = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	float MaxSpreadAngleDegrees = 135.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	float MaxDownwardAngleDegrees = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	TSubclassOf<AActor> TargetComponentClass;

	UPROPERTY(BlueprintReadOnly, Category = "Targeted Projectile Class")
	TObjectPtr<USceneComponent> TargetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	float TargetComponentLifeSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeted Projectile Class")
	FGameplayTag TargetEnemiesStateTag;

private:
	UFUNCTION()
	virtual void FireProjectile() override;
	virtual FVector GetFireDirection(FVector SpawnLocation) override;
	virtual USceneComponent* GetTargetComponent() override;

	FTimerHandle ProjectileTimerHandle;

	int32 ProjectilesFired = 0;
};