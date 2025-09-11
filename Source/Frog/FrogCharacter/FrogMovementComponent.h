// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FrogMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None UMETA(DisplayName = "None"),
	CMOVE_Grapple UMETA(DisplayName = "Grapple"),
	CMOVE_Downed UMETA(DisplayName = "Downed"),
	CMOVE_Floating UMETA(DisplayName = "Floating"),
};

/**
 * 
 */
UCLASS()
class FROG_API UFrogMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	
protected:
	void PhysGrapple(float DeltaTime, int32 Iterations);
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
};
