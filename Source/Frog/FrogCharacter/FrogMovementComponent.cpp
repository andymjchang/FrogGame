// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogMovementComponent.h"

#include "FrogCharacter.h"

void UFrogMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)

{
	Super::PhysCustom(DeltaTime, Iterations);
	
	if (CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		PhysGrapple(DeltaTime, Iterations);
	}
	if (CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Downed))
	{
		
	}
	else
	{
		
	}
} 

void UFrogMovementComponent::PhysGrapple(float DeltaTime, int32 Iterations)
{
	if (!CharacterOwner) return;

	AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner);
	if (!Frog || !Frog->GetIsGrapple())
	{
		SetMovementMode(MOVE_Falling);
		return;
	}
	
	const FVector GrappleDirection = (Frog->GetGrapplePoint() - Frog->GetActorLocation()).GetSafeNormal();
	const float GrappleDistance = (Frog->GetGrapplePoint() - Frog->GetActorLocation()).Size();
    
	// Apply grapple acceleration
	const FVector GrappleAcceleration = GrappleDirection * Frog->GetGrappleAcceleration();
	if (GrappleDistance > 50.f) Velocity += GrappleAcceleration * DeltaTime;

	// Clamp speed 
	const FVector GrappleGravityDirection = GetGravityDirection();
	const float DotProduct = FVector::DotProduct(Velocity.GetSafeNormal(), GrappleGravityDirection);
	bool bMovingTowardsGravity = DotProduct > 0.0f;
	float SpeedLimit;
	if (bMovingTowardsGravity)
	{
		SpeedLimit = 4000.f; // Default physics terminal velocity
	}
	else
	{
		SpeedLimit = GetMaxSpeed();
	}
	//Velocity = NewFallVelocity(Velocity, -GetGravityDirection() * GetGravityZ(), DeltaTime); 
	if (Velocity.Size() > SpeedLimit) Velocity = Velocity.GetSafeNormal() * GetMaxSpeed();
    
	// Move with sliding collision
	const FVector Delta = Velocity * DeltaTime;
	FHitResult Hit;
	MoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, &Hit);
    
	if (Hit.bBlockingHit)
	{
		HandleImpact(Hit, DeltaTime, Delta);
		SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, false);
	}
    
	// Tongue Visuals
	const FRotator NewGrappleRotation = GrappleDirection.Rotation() + FRotator(-30, 0, 0);
	Frog->SetActorRotation(NewGrappleRotation);
}

void UFrogMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner);
	if (!Frog) return;
	
	// Enter Grapple
	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		Frog->SetTongueVisibility(true);
		return;
	}
    
	// Exit Grapple
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	{
		Frog->SetTongueVisibility(false);
		return;
	}
}
