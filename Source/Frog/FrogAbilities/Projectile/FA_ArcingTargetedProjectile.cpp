// FrogTargetedProjectile.cpp

#include "FA_ArcingTargetedProjectile.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter/EnemyCharacter.h"
#include "FrogCharacter/FrogCharacter.h"

void UFA_ArcingTargetedProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
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
            TargetComponent = Cast<USceneComponent>(Enemy->GetHitboxComponent());
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
        &UFA_ArcingTargetedProjectile::FireProjectile,
        DelayBetweenShots,
        true,
        0.0f 
    );
}

void UFA_ArcingTargetedProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFA_ArcingTargetedProjectile::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
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

void UFA_ArcingTargetedProjectile::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
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

void UFA_ArcingTargetedProjectile::FireProjectile()
{
    ProjectilesFired++;

    Super::FireProjectile();
    
    if (ProjectilesFired >= NumProjectiles)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
        }
    }
    
}

FVector UFA_ArcingTargetedProjectile::GetFireDirection(FVector SpawnLocation)
{
    const FVector BaseFireDirection = (GetCrosshairLocation(false) - GetSpawnLocation()).GetSafeNormal();
    const float RandomAngle = FMath::FRandRange(MinSpreadAngleDegrees, MaxSpreadAngleDegrees);
    const FVector RotationAxis = FVector::CrossProduct(BaseFireDirection, FMath::VRand()).GetSafeNormal();
    
    return BaseFireDirection.RotateAngleAxis(RandomAngle, RotationAxis);
}

USceneComponent* UFA_ArcingTargetedProjectile::GetTargetComponent()
{
    return TargetComponent;
}


