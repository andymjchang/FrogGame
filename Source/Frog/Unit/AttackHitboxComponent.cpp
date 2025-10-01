// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackHitboxComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "NiagaraFunctionLibrary.h"

UAttackHitboxComponent::UAttackHitboxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanEverAffectNavigation = false;
	
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(true);
}

void UAttackHitboxComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UAttackHitboxComponent::OnHitboxOverlapBegin);

	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		OwningAbilitySystem = AbilitySystemInterface->GetAbilitySystemComponent();
	}

	if (!GetOwner()->HasAuthority())
	{
		SetGenerateOverlapEvents(false);
	}

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UAttackHitboxComponent::ResetHitActors()
{
	HitActors.Empty();
}

void UAttackHitboxComponent::OnHitboxOverlapBegin(UPrimitiveComponent* overlapped_component, AActor* other_actor,
	UPrimitiveComponent* other_comp, int32 other_body_index, bool b_from_sweep, const FHitResult& sweep_result)
{
	if (other_actor && other_actor != GetOwner() && GetOwner()->HasAuthority())
	{
		if (!HitActors.Contains(other_actor))
		{
			HitActors.Add(other_actor);
			ApplyGameplayEffect(other_actor, GameplayEffect);
		}
	}
}

void UAttackHitboxComponent::ApplyGameplayEffect(const AActor* OtherActor, const TSubclassOf<UGameplayEffect> Effect)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		UAbilitySystemComponent* TargetASC = AbilitySystemInterface->GetAbilitySystemComponent();
		if (GameplayEffect && OwningAbilitySystem.IsValid() && TargetASC)
		{
			FGameplayEffectContextHandle Context = OwningAbilitySystem->MakeEffectContext();
			Context.AddSourceObject(this);
			Context.AddInstigator(GetOwner(), GetOwner());
			FGameplayEffectSpecHandle Spec = OwningAbilitySystem->MakeOutgoingSpec(Effect, 1.0f, Context);
            
			if (Spec.IsValid())
			{ 
				OwningAbilitySystem->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
			}
		}
	}
}

void UAttackHitboxComponent::SpawnDestroyVFX(const FVector& OtherActorLocation) const
{
	if (!HitFX) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		HitFX,
		OtherActorLocation,
		FRotator::ZeroRotator,
		FVector::OneVector,
		true,
		true
	);
}
