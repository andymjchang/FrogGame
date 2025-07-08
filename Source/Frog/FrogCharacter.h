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
	
protected: // Members NO FUNCTIONS HERE
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
	UPROPERTY(VisibleAnywhere, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	UUnitAttributeSet* UnitAttributes;
	UPROPERTY(EditAnywhere, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	UAbilitySet* AbilitySet;
	UPROPERTY(EditAnywhere, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FAbilityInputBindings AbilityInputBindings;
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;

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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Gameplay Ability System
	UFUNCTION(Server, Reliable)
	void ServerGrapple();
	UFUNCTION(Server, Reliable)
	void ServerStopGrapple();

protected: // Functions
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
	bool GetGrapplePoint();
	void ApplyGrappleForce(float DeltaSeconds);

	// Abilities
	void SetupAbilities();


public: // Inline Functions
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Tongue cable object
	FORCEINLINE class UFrogTongue* GetTongue() const { return Tongue; }
};

