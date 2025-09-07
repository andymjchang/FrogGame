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
    CollisionComponent->SetCollisionProfileName(TEXT("ProjectileWorldCollider"));
    RootComponent = CollisionComponent;

    EffectCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EffectCollisionComponent"));
    EffectCollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));
    EffectCollisionComponent->SetupAttachment(RootComponent);

    EffectCollisionComponent2 = CreateDefaultSubobject<USphereComponent>(TEXT("EffectCollisionComponent2"));
    EffectCollisionComponent2->SetCollisionProfileName(TEXT("NoCollision"));
    EffectCollisionComponent2->SetupAttachment(RootComponent);

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

void AProjectile::SetOwningAbilitySystem(UAbilitySystemComponent* InputAbilitySystem)
{
    this->OwningAbilitySystem = InputAbilitySystem;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    EffectCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentBeginOverlap);
    if (bIsSecondHitboxActive)
    {
        EffectCollisionComponent2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        EffectCollisionComponent2->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentBeginOverlap2); 
    }
}

void AProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ApplyGameplayEffect(OtherActor, GameplayEffect, bDestroyOnHitbox1Hit);
}

void AProjectile::OnComponentBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ApplyGameplayEffect(OtherActor, GameplayEffect2, bDestroyOnHitbox2Hit);
}

void AProjectile::ApplyGameplayEffect(const AActor* OtherActor, const TSubclassOf<UGameplayEffect> Effect, const bool bDestroyOnHit)
{
    if (bApplyEffect && OtherActor && OtherActor != GetOwner())
    {
        if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
        {
            UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
            if (GameplayEffect && OwningAbilitySystem.IsValid() && TargetASC)
            {
                FGameplayEffectContextHandle Context = OwningAbilitySystem->MakeEffectContext();
                Context.AddSourceObject(this);
                Context.AddInstigator(GetInstigator(), GetOwner()); 
                FGameplayEffectSpecHandle Spec = OwningAbilitySystem->MakeOutgoingSpec(Effect, 1.0f, Context);
                
                if (Spec.IsValid())
                { 
                    OwningAbilitySystem->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
                }
            }
        }
        if (bDestroyOnHit) Destroy();
        SpawnDestroyVFX();
    }
        
    
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

