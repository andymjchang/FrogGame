// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Frog/GAS/UnitAttributeSet.h"
#include "Unit/NametagWidgetComponent.h"
#include "Unit/ProjectileSpawnerComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComponent"));
	Hitbox->SetCollisionProfileName(TEXT("EnemyHitbox"));
	Hitbox->SetupAttachment(RootComponent);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UUnitAttributeSet>(TEXT("AttributeSet"));

	HealthBarWidgetComponent = CreateDefaultSubobject<UNametagWidgetComponent>(TEXT("NametagWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawnerComponent>(TEXT("ProjectileSpawner"));
}

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

UProjectileSpawnerComponent* AEnemyCharacter::GetProjectileSpawnerComponent()
{
	return ProjectileSpawner;
}

