// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "AttackHitboxComponent.generated.h"


class UNiagaraSystem;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UAttackHitboxComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UAttackHitboxComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ResetHitActors();

	UFUNCTION()
	void OnHitboxOverlapBegin(UPrimitiveComponent* overlapped_component, AActor* other_actor,
							  UPrimitiveComponent* other_comp, int32 other_body_index, bool b_from_sweep,
							  const FHitResult& sweep_result);
	
	void ApplyGameplayEffect(const AActor* OtherActor, TSubclassOf<UGameplayEffect> Effect);
	void SpawnDestroyVFX(const FVector& OtherActorLocation) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> HitFX;

	UPROPERTY()
	TSet<AActor*> HitActors;
	
	TWeakObjectPtr<UAbilitySystemComponent> OwningAbilitySystem;
};
