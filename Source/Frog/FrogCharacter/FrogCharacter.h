// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputAction.h"
#include "Unit/UnitInterface.h"
#include "GAS/FrogAbilitySystem.h"
#include "AbilitySystemInterface.h"
#include "FrogCharacter.generated.h"

class UNametagWidgetComponent;
class UWidgetComponent;
class UFrogHUD;
class UAbilitySet;
class UUnitAttributeSet;
class AProjectile;
enum class EAbilityInputID : uint8;
struct FInputActionValue;
class UFrogTongue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UFrogAbilitySystem;
class UProjectileSpawnerComponent;


/**
 * Struct that ties enhanced input actions to GAS ability inputs
 */
USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID = EAbilityInputID::None;
};

/**
 * Contains array of ability inputs for a GAS character
 */
USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;
};

UCLASS(config=Game)
class AFrogCharacter : public ACharacter, public IUnitInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: /* Public Functions */
	explicit AFrogCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/// Unit Interface 
	virtual void HandleDeath() override;
	virtual void SetHealth(const float NewHealth) override;
	virtual void SetMaxHealth(const float NewMaxHealth) override;
	virtual UProjectileSpawnerComponent* GetProjectileSpawnerComponent() override;

	void SetTongueVisibility(bool Value) const;      

public: /* Public Members */

protected: /* Protected Functions */
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual bool CanJumpInternal_Implementation() const override;

	// GAS
	void AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID);
	void AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID);
	void SetupAbilities();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Grapple 
	void RedrawTongueLocation() const;
	
protected: /* Members */
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Hitbox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFrogTongue* Tongue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNametagWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileSpawnerComponent* ProjectileSpawner;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UFrogHUD> FrogHUDClass;
	
	UPROPERTY()
	UFrogHUD* FrogHUDWidget;
	
	// Movement
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DiveSpeed;

	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UFrogAbilitySystem* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	UUnitAttributeSet* AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	UAbilitySet* AbilitySet;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	// Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FAbilityInputBindings AbilityInputBindings;
	
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	
	// Grapple 
	UPROPERTY(Replicated)
	bool bIsGrapple;
	
	UPROPERTY(Replicated)
	FVector GrapplePoint;
	
	float GrappleStrength;

public: /* Public Getters/Setters */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UFrogTongue* GetTongue() const { return Tongue; }
	FORCEINLINE bool GetIsGrapple() const { return bIsGrapple; }
	FORCEINLINE void SetIsGrapple(const bool bNewIsGrapple) { bIsGrapple = bNewIsGrapple; }
	FORCEINLINE float GetGrappleStrength() const { return GrappleStrength; }
	FORCEINLINE void SetGrappleStrength(const float NewGrappleStrength) { GrappleStrength = NewGrappleStrength; }
	FORCEINLINE FVector GetGrapplePoint() const { return GrapplePoint; }
	FORCEINLINE void SetGrapplePoint(const FVector& NewGrapplePoint) { GrapplePoint = NewGrapplePoint; }
	FORCEINLINE float GetWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE float GetDiveSpeed() const { return DiveSpeed; }
};

