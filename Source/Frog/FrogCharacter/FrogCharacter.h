// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputAction.h"
#include "Unit/UnitInterface.h"
#include "GAS/FrogAbilitySystem.h"
#include "AbilitySystemInterface.h"
#include "GAS/FrogAttributeSet.h"
#include "FrogCharacter.generated.h"

class USphereComponent;
class UNametagWidgetComponent;
class UWidgetComponent;
class UFrogHUD;
class UAbilitySet;
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

	void SetTongueVisibility(bool Value);

	UFUNCTION(BlueprintCallable)
	FVector GetFloorLocation();

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
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	void SetupAbilities();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Grapple 
	void RedrawTongueLocation();

	// Find Enemy Under Crosshair
	void FindEnemyUnderCrosshair();
	virtual void OnTargetEnemyTagChanged(const FGameplayTag Tag, int32 NewCount);

	// Track applied gameplay effects
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageTag;

	FDelegateHandle GameplayTagAppliedHandle;
	void OnGameplayEffectApplied(UAbilitySystemComponent* InputAbilitySystem,
	                             const FGameplayEffectSpec& GameplayEffectSpec,
	                             FActiveGameplayEffectHandle ActiveGameplayEffectSpec);

protected: /* Members */
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Hitbox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFrogTongue> Tongue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNametagWidgetComponent> HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileSpawnerComponent> ProjectileSpawner;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UFrogHUD> FrogHUDClass;
	
	UPROPERTY()
	TObjectPtr<UFrogHUD> FrogHUDWidget;
	
	// Movement
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DiveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DownedSpeed;

	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UFrogAbilitySystem> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	TObjectPtr<UFrogAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> OnDeathEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Targeting")
	float FindEnemyUnderCrosshairTraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Targeting")
	float FindEnemyUnderCrosshairTraceDistance;

	UPROPERTY(EditAnywhere, Category = "Enemy Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> FindEnemyUnderCrosshairObjectType;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Targeting")
	FGameplayTag FindEnemyUnderCrosshairGameplayTag;

	UPROPERTY(Replicated)
	bool bFindEnemyUnderCrosshair;

	FDelegateHandle TargetEnemyTagDelegateHandle;

	TWeakObjectPtr<AActor> TargetEnemyActor;
	
	// Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FAbilityInputBindings AbilityInputBindings;
	
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
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
	FORCEINLINE void SetFindEnemyUnderCrosshair(const bool Value) { bFindEnemyUnderCrosshair = Value; }
	FORCEINLINE AActor* GetTargetEnemyActor() const { return TargetEnemyActor.Get(); }
};

