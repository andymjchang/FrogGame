// Fill out your copyright notice in the Description page of Project Settings.


#include "FA_Projectile.h"

// FrogTargetedProjectile.cpp

#include "FA_ArcingTargetedProjectile.h"
#include "TimerManager.h"
#include "FrogCharacter/FrogCharacter.h" 
#include "Unit/ProjectileSpawnerComponent.h"
#include "Unit/UnitInterface.h"

void UFA_Projectile::FireProjectile(const FVector& SpawnLocation, const FRotator& FireRotation, const FVector& FireDirection)
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

void UFA_Projectile::FireProjectile()
{
    FireProjectile(GetSpawnLocation(), GetFireRotation(), GetFireDirection(GetSpawnLocation()));
}

FVector UFA_Projectile::GetSpawnLocation()
{
    return GetActorInfo().AvatarActor.Get()->GetActorLocation() + FVector(0, 0, 50.f);
}

FRotator UFA_Projectile::GetFireRotation()
{
    return FRotator::ZeroRotator;
}

FVector UFA_Projectile::GetFireDirection(FVector SpawnLocation)
{
    return (GetCrosshairLocation(false) - SpawnLocation).GetSafeNormal();
}

USceneComponent* UFA_Projectile::GetTargetComponent()
{
    return nullptr;
}
