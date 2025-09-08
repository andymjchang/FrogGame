// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAbility.h"

#include "Camera/CameraComponent.h"
#include "FrogCharacter/FrogCharacter.h"

UFrogAbility::UFrogAbility() : CooldownDuration(0), bEndAbilityOnInputReleased(true), bRecastIfHeld(false),
                               bIsHeld(false)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Frog.State.Downed")));
}

void UFrogAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bIsHeld = true;
}

void UFrogAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	bIsHeld = false;
	if (bEndAbilityOnInputReleased) EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

const FGameplayTagContainer* UFrogAbility::GetCooldownTags() const
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

void UFrogAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		if (bRecastIfHeld && CooldownTags.Num() > 0)
		{
			const FGameplayTag CooldownTag = *CooldownTags.CreateConstIterator();
			
			CooldownEventHandle = ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
									 .AddUObject(this, &UFrogAbility::OnCooldownTagChanged);
		}
	}
}

void UFrogAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
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

void UFrogAbility::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		if (bRecastIfHeld && bIsHeld)
		{
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
			{
				ASC->TryActivateAbility(CurrentSpecHandle);
			}
		}
	}
}

void UFrogAbility::AttemptReactivation()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->TryActivateAbility(CurrentSpecHandle);
	}
}

void UFrogAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Frog.Cooldown")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

FVector UFrogAbility::GetCrosshairLocation(bool bGetClosestTarget) const
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
