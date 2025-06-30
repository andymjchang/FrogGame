// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FrogCharacter.generated.h"

// Forward declarations
class UFrogTongue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFrogAbilitySystem;

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None        UMETA(DisplayName = "None"),
	Confirm     UMETA(DisplayName = "Confirm"),
	Cancel      UMETA(DisplayName = "Cancel"),
	Fire        UMETA(DisplayName = "Fire"),
	// Add more if needed
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	UFrogTongue* Tongue;

	// Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	UFrogAbilitySystem* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;
	
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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	FVector GrapplePoint;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	bool bIsGrapple;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float CameraGrappleLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float GrappleStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple, meta = (AllowPrivateAccess = "true"))
	float CameraOffset;

public:
	AFrogCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void ApplyGrappleForce(float DeltaSeconds);
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);
	void StopGrapple(const FInputActionValue& Value);

	// Grapple functions
	bool GetGrapplePoint();
	UFUNCTION(Server, Reliable)
	void HandleGrapple(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void HandleStopGrapple(const FInputActionValue& Value);

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Tongue cable object
	FORCEINLINE class UFrogTongue* GetTongue() const { return Tongue; }
};

