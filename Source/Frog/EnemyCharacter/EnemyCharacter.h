// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Unit/UnitInterface.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTreeComponent;
class UBehaviorTree;
class UBlackboardComponent;
class UAttributeSet;
class UCapsuleComponent;
class UProjectileSpawnerComponent;
class UNametagWidgetComponent;

UCLASS()
class FROG_API AEnemyCharacter : public ACharacter, public IUnitInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: // Public Functions
	AEnemyCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Unit Interface
	virtual void HandleDeath() override;
	virtual void SetHealth(const float NewHealth) override;
	virtual void SetMaxHealth(const float NewMaxHealth) override;
	virtual UProjectileSpawnerComponent* GetProjectileSpawnerComponent() override;

	FORCEINLINE UCapsuleComponent* GetHitboxComponent() { return Hitbox; }

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() { return BehaviorTreeAsset; }

protected: // Protected Function
	virtual void BeginPlay() override;
	
protected: // Protected Members
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> Hitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNametagWidgetComponent> HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileSpawnerComponent> ProjectileSpawner;
	
	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAttributeSet> AttributeSet;

	// AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset	;

};
