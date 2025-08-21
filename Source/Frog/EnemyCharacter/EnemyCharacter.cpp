// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Frog/GAS/UnitAttributeSet.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HitboxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComponent"));
	HitboxComponent->SetCollisionProfileName(TEXT("EnemyHitbox"));
	HitboxComponent->SetupAttachment(RootComponent);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UUnitAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::HandleDeath()
{
	AbilitySystemComponent->CancelAllAbilities();
	Destroy();
}

void AEnemyCharacter::SetHealth(const float NewHealth)
{
}

void AEnemyCharacter::SetMaxHealth(const float NewMaxHealth)
{
}

