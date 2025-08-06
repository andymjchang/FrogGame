// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Frog/GAS/FrogGameplayAbility.h"
#include "FrogAbilityGrapple.generated.h"

class AFrogCharacter;
class UCameraComponent;

UCLASS()
class FROG_API UFrogAbilityGrapple : public UFrogGameplayAbility
{
    GENERATED_BODY()

public:
    UFrogAbilityGrapple();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                           const FGameplayAbilityActorInfo* ActorInfo,
                           const FGameplayAbilityActivationInfo ActivationInfo,
                           bool bReplicateEndAbility,
                           bool bWasCancelled) override;

    virtual void InputReleased(const FGameplayAbilitySpecHandle Handle,
                              const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo) override;

    void PerformGrapple(AFrogCharacter* Character, const FVector& GrapplePoint) const;
    void PerformStopGrapple(AFrogCharacter* Character);

    /**
     * Traces from camera to mouse position and then from player to that position
     * @return FVector GrapplePoint in world position
     */
    FVector TraceGrapplePoint(AActor* CastingActor, UCameraComponent* FollowCamera, float CameraGrappleLength) const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grapple")
    float GrappleRange;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grapple")
    float GrappleStrength;

};