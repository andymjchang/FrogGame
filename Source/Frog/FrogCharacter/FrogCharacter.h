// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputAction.h"
#include "GAS/FrogAbilitySystem.h"
#include "AbilitySystemInterface.h"
#include "GAS/FrogAttributeSet.h"
#include "FrogCharacter.generated.h"

class IItemInterface;
class UInteractableWidgetComponent;
class IInteractableInterface;
class UBoxComponent;
class USphereComponent;
class UNametagWidgetComponent;
class UWidgetComponent;
class UAbilitySet;
class UContainerComponent;
class UItemData;
enum class EAbilityInputID : uint8;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UFrogAbilitySystem;


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
class FROG_API AFrogCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public: /* Public Functions */
	explicit AFrogCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: /* Getters/Setters */
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE IInteractableInterface* GetClosestInteractable() const { return ClosestInteractable.Get(); }
	FORCEINLINE UContainerComponent* GetContainerComponent() const { return ContainerComponent; }

protected: /* Protected Functions */
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// GAS Initialization
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void Move(const FInputActionValue& Value);
	void UpdateClosestInteractable();
	
	UFUNCTION()
	void HandleAddedToInventory(const TScriptInterface<IItemInterface>& Item);
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex);
	
	// GAS
	void AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID);
	void AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID);
	void SetupAbilities();

protected: /* Members */
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UAbilitySet> AbilitySet;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UMaterialInterface> HighlightMaterial;	
		
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> PlayerItemData;
	
	// GamePlay Interactions
	TArray<TWeakInterfacePtr<IInteractableInterface>> OverlappingInteractableArray;
	TWeakInterfacePtr<IInteractableInterface> ClosestInteractable;
	
protected: // Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNametagWidgetComponent> NametagWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UContainerComponent> ContainerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractableWidgetComponent> InventoryWidgetComponent;	

	// HitBoxes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> InteractHitbox;
	
protected:	// Gameplay Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	TObjectPtr<UFrogAbilitySystem> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
	TObjectPtr<UFrogAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability System")
	FAbilityInputBindings AbilityInputBindings;
	
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitialAbilitySpecHandles;
	
protected: // Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;


};

