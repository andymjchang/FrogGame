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

	const FVector GrappleVector = (Frog->GetGrapplePoint() - Frog->GetActorLocation());
	const FVector GrappleDirection = GrappleVector.GetSafeNormal();
    
	// Apply gravity (like Flying mode does)
	// Velocity.Z += GetGravityZ() * DeltaTime;
    
	// Apply grapple acceleration (similar to LaunchCharacter effect)
	const FVector GrappleAcceleration = GrappleDirection * Frog->GetGrappleStrength();
	Velocity += GrappleAcceleration * DeltaTime;
    
	// Apply air resistance/drag if desired (Flying mode has this)
	// Velocity *= FMath::Max(0.f, 1.f - BrakingDecelerationFlying * DeltaTime / FMath::Max(1.f, Velocity.Size()));
    
	// Move the character
	const FVector Delta = Velocity * DeltaTime;
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

	// Tongue Visuals
	FRotator NewGrappleRotation = GrappleDirection.Rotation() + FRotator(-30, 0, 0);
	Frog->SetActorRotation(NewGrappleRotation);
	Frog->SetTongueEndPosition();
}

void UFrogMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
    
	// Enter Grapple
	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		if (AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner)) Frog->SetTongueVisibility(true);
	}
    
	// Exit Grapple
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		if (AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner)) Frog->SetTongueVisibility(false);
	}
}
