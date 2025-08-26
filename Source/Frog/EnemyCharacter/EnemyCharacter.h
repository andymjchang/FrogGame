// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Unit/UnitInterface.h"
#include "EnemyCharacter.generated.h"

class UProjectileSpawnerComponent;
class UNametagWidgetComponent;

UCLASS()
class FROG_API AEnemyCharacter : public ACharacter, public IUnitInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: /// Public Functions
	AEnemyCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/// Unit Interface
	virtual void HandleDeath() override;
	virtual void SetHealth(const float NewHealth) override;
	virtual void SetMaxHealth(const float NewMaxHealth) override;
	virtual UProjectileSpawnerComponent* GetProjectileSpawnerComponent() override;

protected: /// Protected Function
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected: /// Protected Members
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Hitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNametagWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileSpawnerComponent* ProjectileSpawner;
	
	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	class UAttributeSet* AttributeSet;

};
