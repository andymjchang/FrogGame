// Fill out your copyright notice in the Description page of Project Settings.

#include "HomingProjectile.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "EnemyCharacter/EnemyCharacter.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AHomingProjectile::AHomingProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AHomingProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(
        ScanTimerHandle,       
        this,                
        &AHomingProjectile::ScanForTarget, 
        ScanRate,           
        true,          
        0.0f    
    );
}

void AHomingProjectile::ScanForTarget()
{
    const FVector TraceStart = GetActorLocation() + GetActorForwardVector() * ScanRadius;
    const FVector TraceEnd = TraceStart;
 
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    
    FHitResult HitResult;
    
    const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
        GetWorld(),            
        TraceStart,            
        TraceEnd,             
        ScanRadius,            
        TargetObjectTypes,         
        false,                
        ActorsToIgnore,        
        EDrawDebugTrace::ForOneFrame,
        HitResult,           
        true
    );
    
    if (bHit)
    {
        if (AEnemyCharacter* TargetActor = Cast<AEnemyCharacter>(HitResult.GetActor()))
        {
            if (USceneComponent* TargetComponent = Cast<USceneComponent>(TargetActor->GetHitboxComponent()))
            {
                ProjectileMovement->HomingTargetComponent = TargetComponent;
                ProjectileMovement->bIsHomingProjectile = true;
                GetWorld()->GetTimerManager().ClearTimer(ScanTimerHandle);
            }
        }
    }
}