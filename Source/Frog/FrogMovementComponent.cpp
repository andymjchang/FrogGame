// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogMovementComponent.h"

#include "FrogCharacter.h"

void UFrogMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations) 
{
	if (CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		PhysGrapple(DeltaTime, Iterations);
	}
	else
	{
		Super::PhysCustom(DeltaTime, Iterations);
	}
}

void UFrogMovementComponent::PhysGrapple(float DeltaTime, int32 Iterations)
{
	if (!CharacterOwner) return;

	AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner);
	if (!Frog || !Frog->GetIsGrapple()) return;

	const FVector GrappleVector = (Frog->GetGrapplePoint() - CharacterOwner->GetActorLocation());
	const FVector GrappleDirection = GrappleVector.GetSafeNormal();
	const float GrappleDistance = GrappleVector.Size();

	const FVector GrappleVelocity = GrappleDirection * Frog->GetGrappleStrength();
	const FVector NewLocation = CharacterOwner->GetActorLocation() + GrappleVelocity * DeltaTime;

	const FVector Adjusted = NewLocation - CharacterOwner->GetActorLocation();
	FHitResult Hit;
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
}
