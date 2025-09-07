// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSpawnerComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Actors/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

UProjectileSpawnerComponent::UProjectileSpawnerComponent()
{
}

void UProjectileSpawnerComponent::RequestSpawnProjectile(const TSubclassOf<AProjectile>& ActorClass,
                                                         const FVector& Location, const FRotator& Rotation,
                                                         const FVector& FireDirection, USceneComponent* TargetComponent,
                                                         UAbilitySystemComponent* AbilitySystem)
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;
    
    if (Owner->GetLocalRole() == ROLE_Authority)
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, true, TargetComponent, AbilitySystem);
        MulticastSpawnProjectile(ActorClass, Location, Rotation, FireDirection, TargetComponent, AbilitySystem);
    }
    else // Client prediction
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, false, TargetComponent, AbilitySystem);
    }
}



void UProjectileSpawnerComponent::MulticastSpawnProjectile_Implementation(
    const TSubclassOf<AProjectile> ActorClass, const FVector& Location, const FRotator& Rotation,
    const FVector& FireDirection, USceneComponent* TargetComponent, UAbilitySystemComponent* AbilitySystem)
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;
    
    if (Owner->GetLocalRole() == ROLE_SimulatedProxy)
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, false, TargetComponent, AbilitySystem);
    }
}

void UProjectileSpawnerComponent::SpawnProjectileInternal(const TSubclassOf<AProjectile>& ActorClass,
    const FVector& Location, const FRotator& Rotation, FVector FireDirection, bool bApplyEffect,
    USceneComponent* TargetComponent, UAbilitySystemComponent* AbilitySystem)
{
    AActor* MyOwner = GetOwner();
    if (!GetWorld() || !MyOwner || !ActorClass) return;

    FActorSpawnParameters Params;
    Params.Owner = Params.Instigator = Cast<APawn>(MyOwner);
    Params.CustomPreSpawnInitalization = [this, FireDirection, bApplyEffect, TargetComponent, AbilitySystem](AActor* Actor)
    {
        if (const auto Projectile = Cast<AProjectile>(Actor))
        {
            Projectile->FireInDirection(FireDirection);
            Projectile->SetApplyEffect(bApplyEffect);
            Projectile->SetOwningAbilitySystem(AbilitySystem);
            
            if (TargetComponent)
            {
                Projectile->GetProjectileMovementComponent()->HomingTargetComponent = TargetComponent ;
            }
        }
    };
    
    GetWorld()->SpawnActor<AProjectile>(ActorClass, Location, Rotation, Params);
}

