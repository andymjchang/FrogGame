// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "UnitAttributeSet.h"
#include "GameplayAbilitySet.h"
#include "FrogCharacter.generated.h"

// Forward declarations
class UFrogTongue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFrogAbilitySystem;

USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID AbilityInputID;
};

USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};

UCLASS(config=Game)
class AFrogCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected: /* Members NO FUNCTIONS HERE */
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grapple)
	UFrogTongue* Tongue;

	// Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UFrogAbilitySystem* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability System")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	UUnitAttributeSet* UnitAttributes;
	UPROPERTY(EditAnywhere, Category = "Ability System")
	UAbilitySet* AbilitySet;
	UPROPERTY(EditAnywhere, Category = "Ability System")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FAbilityInputBindings AbilityInputBindings;
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* GrappleAction;

	// Networking
	FTimerHandle ClientAuthoritativeTimerHandle;
	
	// Grapple
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Grapple)
	bool bIsGrapple;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Grapple)
	FVector GrapplePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple)
	float CameraGrappleLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple)
	float GrappleStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grapple)
	float CameraOffset;
	UPROPERTY(ReplicatedUsing = OnRep_GrappleRotation, EditAnywhere, BlueprintReadWrite, Category = Grapple)
	FRotator GrappleRotation;

public: /* Public Functions */
	AFrogCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Gameplay Ability System
	UFUNCTION(Server, Reliable)
	void ServerGrapple(const FVector NewGrapplePoint);
	UFUNCTION(Server, Reliable)
	void ServerStopGrapple();

	void SetTongueEndPosition();
	void SetTongueVisibility(bool Value);

protected: /* Protected Functions */
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);
	void StopGrapple(const FInputActionValue& Value);
	
	// Connecting Ability Inputs to GAS
	void AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID);
	void AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID);
	
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Grapple functions
	bool TraceGrapplePoint();
	void ApplyGrappleForce(float DeltaSeconds);
	UFUNCTION()
	void OnRep_GrappleRotation();
	void SetClientAuthoritativeMovement(bool Value);

	// Abilities
	void SetupAbilities();


public: /* Public Members and Getters */
	/** Getters **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UFrogTongue* GetTongue() const { return Tongue; }
	FORCEINLINE bool GetIsGrapple() const { return bIsGrapple; }
	FORCEINLINE void SetIsGrapple(bool bNewIsGrapple) { bIsGrapple = bNewIsGrapple; }
	FORCEINLINE float GetGrappleStrength() { return GrappleStrength; }
	FORCEINLINE FVector GetGrapplePoint() const { return GrapplePoint; }
	
	// Public Members
};

