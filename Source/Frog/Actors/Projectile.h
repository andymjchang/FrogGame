// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraSystem;

UCLASS()
class FROG_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	void FireInDirection(const FVector& ShootDirection) const;
	void SetApplyEffect(bool ApplyEffect);
	
protected: /// Protected Functions
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse,
	           const FHitResult& Hit);
	void SpawnDestroyVFX() const;

protected: /// Protected Members
	bool bApplyEffect;
	/// Combat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float DamageAmount = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* HitFX;

	/// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;
};