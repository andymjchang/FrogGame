// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageAOE.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"


ADamageAOE::ADamageAOE()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	// CollisionComponent->SetCollisionProfileName(TEXT("HitsEnemy"));
	RootComponent = CollisionComponent;
}

void ADamageAOE::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADamageAOE::OnComponentBeginOverlap);

	OwningAbilitySystem = Cast<IAbilitySystemInterface>(GetInstigator())->GetAbilitySystemComponent();
}

void ADamageAOE::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
			if (GameplayEffect && OwningAbilitySystem.IsValid() && TargetASC)
			{
				FGameplayEffectContextHandle Context = OwningAbilitySystem->MakeEffectContext();
				Context.AddSourceObject(this);
				Context.AddInstigator(GetInstigator(), GetOwner()); 
				FGameplayEffectSpecHandle Spec = OwningAbilitySystem->MakeOutgoingSpec(GameplayEffect, 1.0f, Context);
                
				if (Spec.IsValid())
				{ 
					OwningAbilitySystem->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
				}
			}
		}
	}
}

