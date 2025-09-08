// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAbilityProjectile.h"

// FrogTargetedProjectile.cpp

#include "FrogTargetedProjectile.h"
#include "TimerManager.h"
#include "FrogCharacter/FrogCharacter.h" 
#include "Unit/ProjectileSpawnerComponent.h"
#include "Unit/UnitInterface.h"

void UFrogAbilityProjectile::FireProjectile(const FVector& SpawnLocation, const FRotator& FireRotation, const FVector& FireDirection)
{
    const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
        return;
    }
    
    IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
    if (Unit && Unit->GetProjectileSpawnerComponent() && ProjectileClass)
    {
        UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent();
        
        USceneComponent* TargetComponent = GetTargetComponent();
        if (UAbilitySystemComponent* OwningAbilitySystem = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
        {
            ProjectileSpawner->RequestSpawnProjectile(ProjectileClass, SpawnLocation, FireRotation, FireDirection, TargetComponent, OwningAbilitySystem);
        }
    }
}

void UFrogAbilityProjectile::FireProjectile()
{
    FireProjectile(GetSpawnLocation(), GetFireRotation(), GetFireDirection(GetSpawnLocation()));
}

FVector UFrogAbilityProjectile::GetSpawnLocation()
{
    return GetActorInfo().AvatarActor.Get()->GetActorLocation() + FVector(0, 0, 50.f);
}

FRotator UFrogAbilityProjectile::GetFireRotation()
{
    return FRotator::ZeroRotator;
}

FVector UFrogAbilityProjectile::GetFireDirection(FVector SpawnLocation)
{
    return (GetCrosshairLocation(false) - SpawnLocation).GetSafeNormal();
}

USceneComponent* UFrogAbilityProjectile::GetTargetComponent()
{
    return nullptr;
}
