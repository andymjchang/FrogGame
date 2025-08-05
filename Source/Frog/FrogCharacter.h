// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "UnitAttributeSet.h"
#include "GameplayAbilitySet.h"
#include "Projectile.h"
#include "FrogCharacter.generated.h"

class UFrogTongue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFrogAbilitySystem;

/**
 * Struct that ties enhanced input actions to GAS ability inputs
 */
USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID AbilityInputID;
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
class AFrogCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public: /* Public Functions */
	explicit AFrogCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void SetTongueVisibility(bool Value) const;
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(TSubclassOf<AProjectile> ActorClass, const FVector& Location, const FRotator& Rotation);

protected: /* Protected Functions */
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// GAS
	void AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID);
	void AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID);
	void SetupAbilities();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Grapple functions 
	void RedrawTongueLocation(float DeltaSeconds) const;

	// Spawn Projectile Functions
	void SpawnPredictedProjectileInternal(const TSubclassOf<AProjectile>& ActorClass,
		const FVector& Location, const FRotator& Rotation, FVector FireDirection, bool bIsVisualOnly);

	UFUNCTION(Server, Reliable)
	void ServerSpawnPredictedProjectile(TSubclassOf<AProjectile> ActorClass,
		const FVector& Location, const FRotator& Rotation, FVector FireDirection, int32 InstigatorPlayerId);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnPredictedProjectile(TSubclassOf<AProjectile> ActorClass,
		const FVector& Location, const FRotator& Rotation, FVector FireDirection, int32 InstigatorPlayerId);
	
protected: /* Members */
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grapple)
	UFrogTongue* Tongue;

	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UFrogAbilitySystem* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;
	
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

public: /* Public Members and Getters */
	// Getters 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UFrogTongue* GetTongue() const { return Tongue; }
	FORCEINLINE bool GetIsGrapple() const { return bIsGrapple; }
	FORCEINLINE void SetIsGrapple(const bool bNewIsGrapple) { bIsGrapple = bNewIsGrapple; }
	FORCEINLINE float GetGrappleStrength() const { return GrappleStrength; }
	FORCEINLINE void SetGrappleStrength(const float NewGrappleStrength) { GrappleStrength = NewGrappleStrength; }
	FORCEINLINE FVector GetGrapplePoint() const { return GrapplePoint; }
	FORCEINLINE void SetGrapplePoint(const FVector& NewGrapplePoint) { GrapplePoint = NewGrapplePoint; }
};

