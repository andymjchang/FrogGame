// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "HomingProjectile.generated.h"

UCLASS()
class FROG_API AHomingProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AHomingProjectile();

protected:
	virtual void BeginPlay() override;

private:
	void ScanForTarget();
	
	FTimerHandle ScanTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Homing")
	float ScanRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Homing")
	float ScanRate = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Homing", meta = (DisplayName = "Target Object Types"))
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
};