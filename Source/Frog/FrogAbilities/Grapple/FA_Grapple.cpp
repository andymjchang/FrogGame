// Fill out your copyright notice in the Description page of Project Settings.

#include "FA_Grapple.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "Frog/FrogCharacter/FrogCharacter.h"
#include "Frog/FrogCharacter/FrogMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UFA_Grapple::UFA_Grapple()
{
    GrappleRange = 3000.0f;
    GrappleStrength = 4000.f;
}

void UFA_Grapple::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
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
    
    PerformGrapple(Character, GrapplePoint);
}

void UFA_Grapple::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::InputReleased(Handle, ActorInfo, ActivationInfo);
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFA_Grapple::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
    CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
    
    if (AFrogCharacter* Character = Cast<AFrogCharacter>(ActorInfo->AvatarActor.Get()))
    {
        PerformStopGrapple(Character);
    }
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFA_Grapple::PerformGrapple(AFrogCharacter* Character, const FVector& GrapplePoint) const
{
    if (!Character) return;
    
    Character->SetIsGrapple(true);
    Character->SetGrappleStrength(GrappleStrength);
    Character->SetGrapplePoint(GrapplePoint);
    
    if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
    {
        MovementComp->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple));
    }
}

void UFA_Grapple::PerformStopGrapple(AFrogCharacter* Character)
{
    if (!Character) return;
    
    Character->SetIsGrapple(false);
    Character->SetGrapplePoint(FVector::ZeroVector);
    
    if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
    {
        MovementComp->SetMovementMode(MOVE_Falling);
    }
}

FVector UFA_Grapple::TraceGrapplePoint(AActor* CastingActor, UCameraComponent* FollowCamera,
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