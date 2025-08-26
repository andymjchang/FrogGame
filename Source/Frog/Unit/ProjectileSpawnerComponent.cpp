// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSpawnerComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Actors/Projectile.h"

UProjectileSpawnerComponent::UProjectileSpawnerComponent()
{
}

void UProjectileSpawnerComponent::RequestSpawnProjectile(const TSubclassOf<AProjectile>& ActorClass, const FVector& Location, const FRotator& Rotation, const FVector& FireDirection)
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;
    
    if (Owner->GetLocalRole() == ROLE_Authority)
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, true);
        MulticastSpawnProjectile(ActorClass, Location, Rotation, FireDirection);
    }
    else // Client prediction
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, false);
    }
}

void UProjectileSpawnerComponent::MulticastSpawnProjectile_Implementation(const TSubclassOf<AProjectile> ActorClass, const FVector& Location, const FRotator& Rotation, const FVector& FireDirection)
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;
    
    if (Owner->GetLocalRole() == ROLE_SimulatedProxy)
    {
        SpawnProjectileInternal(ActorClass, Location, Rotation, FireDirection, false);
    }
}

void UProjectileSpawnerComponent::SpawnProjectileInternal(const TSubclassOf<AProjectile>& ActorClass, const FVector& Location, const FRotator& Rotation, FVector FireDirection, const bool bApplyEffect) const
{
    AActor* MyOwner = GetOwner();
    if (!GetWorld() || !MyOwner || !ActorClass) return;

    FActorSpawnParameters Params;
    Params.Owner = Params.Instigator = Cast<APawn>(MyOwner);
    Params.CustomPreSpawnInitalization = [this, FireDirection, bApplyEffect](AActor* Actor)
    {
        if (const auto Projectile = Cast<AProjectile>(Actor))
        {
            Projectile->FireInDirection(FireDirection);
            Projectile->SetApplyEffect(bApplyEffect);
        }
    };
    
    GetWorld()->SpawnActor<AProjectile>(ActorClass, Location, Rotation, Params);
}

