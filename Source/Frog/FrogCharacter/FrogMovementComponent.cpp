// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogMovementComponent.h"

#include "FrogCharacter.h"

void UFrogMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)

{
	Super::PhysCustom(DeltaTime, Iterations);
	
	// if (CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple))
	// {
	// 	PhysGrapple(DeltaTime, Iterations);
	// }
	// if (CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Downed))
	// {
	// 	
	// }
	// else
	// {
	// 	
	// }
} 

void UFrogMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	AFrogCharacter* Frog = Cast<AFrogCharacter>(CharacterOwner);
	if (!Frog) return;
}
