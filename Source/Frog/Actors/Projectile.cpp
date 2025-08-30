// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// AProjectile.cpp
AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    InitialLifeSpan = 5.0f;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetGenerateOverlapEvents(true);
    RootComponent = CollisionComponent;

    DamageCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DamageCollisionComponent"));
    DamageCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->SetUpdatedComponent(CollisionComponent);
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectile::FireInDirection(const FVector& ShootDirection) const
{
    ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void AProjectile::SetApplyEffect(const bool ApplyEffect)
{
   this->bApplyEffect = ApplyEffect;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentBeginOverlap);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bApplyEffect)
    {
        if (OtherActor && OtherActor != GetOwner())
        {
            if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
            {
                UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
                if (IsValid(DamageEffect))
                {
                    FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
                    Context.AddSourceObject(this);
                    Context.AddInstigator(GetInstigator(), GetOwner());
                    
                    FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(DamageEffect, 1.0f, Context);
                    if (Spec.IsValid())
                    {
                        TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
                    }
                }
            }
        }
    }
        
    SpawnDestroyVFX();
    Destroy();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    SpawnDestroyVFX();
    Destroy();
}

void AProjectile::SpawnDestroyVFX() const
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        HitFX,
        GetActorLocation(),
        FRotator::ZeroRotator,
        FVector::OneVector,
        true,
        true
    );
}

