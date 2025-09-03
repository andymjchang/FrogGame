// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UGameplayEffect;
class UNiagaraSystem;

UCLASS()
class FROG_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	void FireInDirection(const FVector& ShootDirection) const;
	void SetApplyEffect(bool ApplyEffect);
	
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() { return ProjectileMovement; }
	
protected: // Protected Functions
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse,
	           const FHitResult& Hit);

	void ApplyGameplayEffect(const AActor* OtherActor, TSubclassOf<UGameplayEffect> Effect, bool bDestroyOnHit);
	void SpawnDestroyVFX() const;
	
protected: // Protected Members
	bool bApplyEffect;
	
	// Combat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	bool bDestroyOnHitbox1Hit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> GameplayEffect2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	bool bDestroyOnHitbox2Hit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> HitFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	bool bIsSecondHitboxActive;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> EffectCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> EffectCollisionComponent2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};