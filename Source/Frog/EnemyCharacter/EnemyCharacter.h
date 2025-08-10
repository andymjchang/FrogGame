// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FROG_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: /// Public Functions
	AEnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected: /// Protected Function
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected: /// Protected Members
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* HitboxComponent;
	
	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	class UAttributeSet* AttributeSet;


};
