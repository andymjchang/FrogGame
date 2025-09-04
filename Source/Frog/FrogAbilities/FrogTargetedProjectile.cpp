// FrogTargetedProjectile.cpp

#include "FrogTargetedProjectile.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter/EnemyCharacter.h"
#include "FrogCharacter/FrogCharacter.h"
#include "Unit/ProjectileSpawnerComponent.h"
#include "Unit/UnitInterface.h"

void UFrogTargetedProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
       EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
       return;
    }

    TargetComponent = nullptr;
    if (const AFrogCharacter* Frog = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
    {
        if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Frog->GetTargetEnemyActor()))
        {
            TargetComponent = Cast<USphereComponent>(Enemy->GetHitboxComponent());
        }
    }
    if (!TargetComponent)
    {
        const FVector CrosshairLocation = GetCrosshairLocation(false);
        AActor* DummyActor = GetWorld()->SpawnActor<AActor>(TargetComponentClass, CrosshairLocation, FRotator::ZeroRotator);
        DummyActor->SetLifeSpan(TargetComponentLifeSpawn);
        TargetComponent = DummyActor->GetRootComponent();
    }
    
    ProjectilesFired = 0;
    
    GetWorld()->GetTimerManager().SetTimer(
        ProjectileTimerHandle,
        this,
        &UFrogTargetedProjectile::FireProjectile,
        DelayBetweenShots,
        true,
        0.0f 
    );
}

void UFrogTargetedProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFrogTargetedProjectile::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
    
    if (TargetEnemiesStateTag.IsValid())
    {
        if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
        {
            ASC->AddLooseGameplayTag(TargetEnemiesStateTag);
        }
    }
}

void UFrogTargetedProjectile::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilitySpec& Spec)
{
    Super::OnRemoveAbility(ActorInfo, Spec);
    
    if (TargetEnemiesStateTag.IsValid())
    {
        if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
        {
            ASC->RemoveLooseGameplayTag(TargetEnemiesStateTag);
        }
    }
}

void UFrogTargetedProjectile::FireProjectile()
{
    const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
        return;
    }
    
    ProjectilesFired++;

    if (ProjectilesFired > NumProjectiles)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
        return;
    }
    
    IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
    if (Unit && Unit->GetProjectileSpawnerComponent() && ProjectileClass)
    {
        UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent();
        
        const FVector UnitLocation = ActorInfo->AvatarActor.Get()->GetActorLocation() + FVector(0, 0, 50.f);
        const FVector BaseFireDirection = (GetCrosshairLocation(false) - UnitLocation).GetSafeNormal();
        
        const FVector RandomizedDirection = FMath::VRandCone(BaseFireDirection, FMath::DegreesToRadians(SpreadAngleDegrees));
        const FRotator FireRotation = FRotator(0, 0, 0);
            
        ProjectileSpawner->RequestSpawnProjectile(ProjectileClass, UnitLocation, FireRotation, RandomizedDirection, TargetComponent);
        
    }
}
