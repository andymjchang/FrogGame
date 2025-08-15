// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/UnitAbilitySystemInterface.h"

// AProjectile.cpp
AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    InitialLifeSpan = 5.0f;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->SetSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetGenerateOverlapEvents(true);
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
}

void AProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bApplyEffect)
    {
        if (OtherActor && OtherActor != GetOwner())
        {
            if (const IUnitAbilitySystemInterface* AbilitySystemInterface = Cast<IUnitAbilitySystemInterface>(OtherActor))
            {
                UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
                if (IsValid(DamageEffect))
                {
                    // Create effect context
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
        
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		HitFX,
		GetActorLocation(),
		FRotator::ZeroRotator,
		FVector::OneVector,
		true,
		true
	);
    
    Destroy();
}

