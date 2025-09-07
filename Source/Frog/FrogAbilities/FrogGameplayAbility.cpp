// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogGameplayAbility.h"

#include "Camera/CameraComponent.h"
#include "FrogCharacter/FrogCharacter.h"

UFrogGameplayAbility::UFrogGameplayAbility() : CooldownDuration(0), bRecastIfHeld(false), bIsHeld(false)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Frog.State.Downed")));
}

void UFrogGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bIsHeld = true;
}

void UFrogGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	bIsHeld = false;
	// EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

const FGameplayTagContainer* UFrogGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UFrogGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		if (bRecastIfHeld && CooldownTags.Num() > 0)
		{
			const FGameplayTag CooldownTag = *CooldownTags.CreateConstIterator();
			
			CooldownEventHandle = ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
									 .AddUObject(this, &UFrogGameplayAbility::OnCooldownTagChanged);
		}
	}
}

void UFrogGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		if (bRecastIfHeld && CooldownEventHandle.IsValid() && CooldownTags.Num() > 0)
		{
			const FGameplayTag CooldownTag = *CooldownTags.CreateConstIterator();
			
			ASC->UnregisterGameplayTagEvent(CooldownEventHandle, CooldownTag, EGameplayTagEventType::NewOrRemoved);
			CooldownEventHandle.Reset(); 
		}
	}
}

void UFrogGameplayAbility::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		if (bRecastIfHeld && bIsHeld)
		{
			// Don't try to activate immediately. Instead, set a timer to do it on the next frame.
			// This "untangles" the execution and prevents the re-entrancy bug.
			GetWorld()->GetTimerManager().SetTimer(
				ReactivationTimerHandle,
				this,
				&UFrogGameplayAbility::AttemptReactivation,
				0.01f,  // A tiny delay is enough to push execution to a later frame.
				false
			);
		}
	}
}

void UFrogGameplayAbility::AttemptReactivation()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		// This logic is now safely called in a separate, clean execution context.
		ASC->TryActivateAbility(CurrentSpecHandle);
	}
}

void UFrogGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Frog.Cooldown")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

FVector UFrogGameplayAbility::GetCrosshairLocation(bool bGetClosestTarget) const
{
	AActor* Owner = GetOwningActorFromActorInfo();
	AFrogCharacter* FrogCharacter = Cast<AFrogCharacter>(Owner);
	if (!FrogCharacter) return FVector::ZeroVector;
	UCameraComponent* FollowCamera = FrogCharacter->GetFollowCamera();
	FVector PlayerLocation = FrogCharacter->GetActorLocation();
	
	FVector CameraForward = FollowCamera->GetForwardVector();
	FVector CameraWorldLocation = FollowCamera->GetComponentLocation();
	FVector CameraLookPoint = CameraWorldLocation + CameraForward * 3000.f;
	float CameraDistance = (FollowCamera->GetComponentLocation() - PlayerLocation).Size();
	FVector CameraOffsetLocation = CameraWorldLocation + CameraForward * CameraDistance;
    
	// Line trace from camera to look point
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(FrogCharacter);
	FHitResult CameraHitResult;
	bool bCameraHit = GetWorld()->LineTraceSingleByChannel(
	   CameraHitResult,
	   CameraOffsetLocation,
	   CameraLookPoint,
	   ECC_Visibility,
	   Params
	);

// #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
// 	FColor CameraTraceColor = bCameraHit ? FColor::Red : FColor::Yellow;
// 	DrawDebugLine(
// 		GetWorld(),
// 		CameraOffsetLocation,
// 		bCameraHit ? CameraHitResult.ImpactPoint : CameraLookPoint,
// 		CameraTraceColor,
// 		false,
// 		2.0f,
// 		0,
// 		2.0f
// 	);
// #endif

	FVector CameraImpactPoint;
	if (bCameraHit && bGetClosestTarget)
	{
		CameraImpactPoint = CameraHitResult.ImpactPoint;
	} else
	{
		CameraImpactPoint = CameraWorldLocation + CameraForward * 3000.f;
	}
    
// #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
// 	DrawDebugSphere(
// 		GetWorld(),
// 		CameraImpactPoint,
// 		10.0f,
// 		12,
// 		FColor::Red,
// 		false,
// 		2.0f
// 	);
// #endif

	return CameraImpactPoint;
}
