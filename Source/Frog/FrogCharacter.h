// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Net/UnrealNetwork.h"
#include "CableComponent.h"
#include "FrogCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AFrogCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCableComponent* Tongue;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrappleAction;
	
	// Grapple
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	FVector GrapplePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool bIsGrapple;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float CameraGrappleLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float GrappleStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float CameraOffset;

public:
	AFrogCharacter();
	

protected:
	virtual void Tick(float DeltaSeconds) override;
	void ApplyGrappleForce(float DeltaSeconds);
	virtual void PostInitializeComponents() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);
	void StopGrapple(const FInputActionValue& Value);

	// Grapple functions
	bool GetGrapplePoint();

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Tongue cable object
	FORCEINLINE class UCableComponent* GetTongue() const { return Tongue; }
	
};

