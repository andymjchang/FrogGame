// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// AProjectile.cpp
AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->SetSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    RootComponent = CollisionComponent;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->SetUpdatedComponent(CollisionComponent);
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    // Destroy after 5 seconds
    InitialLifeSpan = 5.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != GetOwner())
    {
        // Check if the hit actor has an Ability System Component
        if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
        {
            UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
            if (DamageEffect)
            {
                // Create effect context
                FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
                Context.AddSourceObject(this);
                Context.AddInstigator(GetInstigator(), GetOwner());

                // Create effect spec
                FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(DamageEffect, 1.0f, Context);
                if (Spec.IsValid())
                {
                    // Set damage amount
                    Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage.Basic"), DamageAmount);
                    
                    // Apply damage
                    TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
                }
            }
        }

        Destroy();
    }
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void AProjectile::ToggleCollision(bool Value)
{
    if (!Value) CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    else CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

