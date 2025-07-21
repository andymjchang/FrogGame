// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogAbilityGrapple.h"
#include "FrogCharacter.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "FrogMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UFrogAbilityGrapple::UFrogAbilityGrapple()
{
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    GrappleRange = 3000.0f;
    GrappleStrength = 4000.f;
}

void UFrogAbilityGrapple::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    // Commit the ability (costs, cooldowns, etc.) - this is predicted automatically
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get());
    if (!Character || !CanGrapple())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Find grapple point
    UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>();
    const FVector GrapplePoint = TraceGrapplePoint(Character, Camera, GrappleRange);
    
    if (GrapplePoint.IsZero())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // This single function call is predicted automatically!
    // - Runs immediately on client for responsiveness
    // - Server validates and potentially corrects
    // - Other clients get the replicated result
    PerformGrapple(Character, GrapplePoint);
}

void UFrogAbilityGrapple::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::InputReleased(Handle, ActorInfo, ActivationInfo);
    
    // This is also predicted automatically
    if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
    {
        PerformStopGrapple(Character);
    }
    
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFrogAbilityGrapple::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
    // Clean up when ability ends
    if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
    {
        PerformStopGrapple(Character);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFrogAbilityGrapple::PerformGrapple(AFrogCharacter* Character, const FVector& GrapplePoint)
{
    if (!Character) return;

    // These are automatically predicted because of NetExecutionPolicy::LocalPredicted
    Character->SetIsGrapple(true);
    Character->SetGrappleStrength(GrappleStrength);
    Character->SetGrapplePoint(GrapplePoint);
    
    if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
    {
        MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple));
    }
}

void UFrogAbilityGrapple::PerformStopGrapple(AFrogCharacter* Character)
{
    if (!Character) return;

    // Also automatically predicted
    Character->SetIsGrapple(false);
    Character->SetGrapplePoint(FVector::ZeroVector);
    
    if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
    {
        MovementComp->SetMovementMode(MOVE_Falling);
    }
}

bool UFrogAbilityGrapple::CanGrapple() const
{
    return true;
}

FVector UFrogAbilityGrapple::TraceGrapplePoint(AActor* CastingActor, UCameraComponent* FollowCamera,
                                              float CameraGrappleLength) const
{
    FVector FinalGrapplePoint = FVector::ZeroVector;
    
    if (!IsValid(FollowCamera) || !IsValid(CastingActor)) 
    {
        return FinalGrapplePoint;
    }

    FVector CastingActorLocation = CastingActor->GetActorLocation();
    
    // Get camera look at point
    FVector CameraWorldLocation = FollowCamera->GetComponentLocation();
    FVector CameraForward = FollowCamera->GetForwardVector();
    FVector CameraLookPoint = CameraWorldLocation + CameraForward * CameraGrappleLength;
    float CameraDistance = (FollowCamera->GetComponentLocation() - CastingActorLocation).Size();
    FVector CameraOffsetLocation = CameraWorldLocation + CameraForward * CameraDistance;
    
    // Line trace from camera to look point
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(CastingActor);
    FHitResult CameraHitResult;
    bool bCameraHit = GetWorld()->LineTraceSingleByChannel(
       CameraHitResult,
       CameraOffsetLocation,
       CameraLookPoint,
       ECC_Visibility,
       Params
    );
    
    // Debug line for camera trace (only in development builds)
    #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
    FColor CameraTraceColor = bCameraHit ? FColor::Red : FColor::Yellow;
    DrawDebugLine(
        GetWorld(),
        CameraOffsetLocation,
        bCameraHit ? CameraHitResult.ImpactPoint : CameraLookPoint,
        CameraTraceColor,
        false,
        2.0f,
        0,
        2.0f
    );
    #endif
    
    if (!bCameraHit)
    {
        return FinalGrapplePoint;
    }

    FVector CameraImpactPoint = CameraHitResult.ImpactPoint;
    
    #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
    DrawDebugSphere(
        GetWorld(),
        CameraImpactPoint,
        10.0f,
        12,
        FColor::Red,
        false,
        2.0f
    );
    #endif

    // Line trace from player to camera's impact point
    FVector PlayerToCameraPoint = CameraImpactPoint - CastingActorLocation;
    PlayerToCameraPoint.Normalize();
    FVector PlayerLookPoint = CastingActorLocation + PlayerToCameraPoint * CameraGrappleLength;
    
    FHitResult PlayerHitResult;
    bool bPlayerHit = GetWorld()->LineTraceSingleByChannel(
       PlayerHitResult,
       CastingActorLocation,
       PlayerLookPoint,
       ECC_Visibility,
       Params
    );
    
    #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
    FColor PlayerTraceColor = bPlayerHit ? FColor::Green : FColor::Orange;
    DrawDebugLine(
        GetWorld(),
        CastingActorLocation,
        bPlayerHit ? PlayerHitResult.ImpactPoint : PlayerLookPoint,
        PlayerTraceColor,
        false,
        2.0f,
        0,
        2.0f
    );
    #endif
    
    if (bPlayerHit)
    {
        FinalGrapplePoint = PlayerHitResult.ImpactPoint;
        
        #if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
        DrawDebugSphere(
            GetWorld(),
            FinalGrapplePoint,
            15.0f,
            12,
            FColor::Green,
            false,
            2.0f
        );
        #endif
        
        return FinalGrapplePoint;
    }
    
    return FinalGrapplePoint;
}