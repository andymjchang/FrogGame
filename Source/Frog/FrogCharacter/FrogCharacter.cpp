// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrogCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "FrogTongue.h"
#include "Net/UnrealNetwork.h"
#include "FrogMovementComponent.h"
#include "Frog/Actors/Projectile.h"
#include "Frog/GAS/AbilitySet.h"
#include "Frog/GAS/UnitAttributeSet.h"
#include "Frog/GAS/FrogAbilities/FrogAbilitySystem.h"
#include "GameFramework/PlayerState.h"

//////////////////////////////////////////////////////////////////////////
// AFrogCharacter

AFrogCharacter::AFrogCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFrogMovementComponent>(AFrogCharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Networking
	bReplicates = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 42.0f);

	// Character Movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1800.0f, 0.0f); 
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 6000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 750.0f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 6000.0f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->BrakingFrictionFactor = 8.f;
	GetCharacterMovement()->BrakingFriction = 8.f;
	GetCharacterMovement()->MaxFlySpeed = 1500.f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 1500.f;

	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;
	
	// Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 

	// Setup Grapple
	Tongue = CreateDefaultSubobject<UFrogTongue>(TEXT("TongueComponent"));
	Tongue->SetupAttachment(GetRootComponent());

	// Frog Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UFrogAbilitySystem>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UUnitAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFrogCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Grapple
	DOREPLIFETIME(AFrogCharacter, GrapplePoint);
	DOREPLIFETIME(AFrogCharacter, bIsGrapple);
}

void AFrogCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFrogCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) SetupAbilities();
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::SetupAbilities()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet)) return;
	if (IsValid(AbilitySet))
	{
		InitialAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}

	// Apply initial stats
	if (IsValid(DefaultAttributes))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(DefaultAttributes->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
	}
	
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UUnitAttributeSet::GetHealthAttribute()).AddUObject(this, &AFrogCharacter::OnHealthAttributeChanged);
}

void AFrogCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFrogCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrogCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrogCharacter::Look);

		// GAS
		for (const auto [InputAction, AbilityInputID] : AbilityInputBindings.Bindings)
		{
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AFrogCharacter::AbilityInputBindingPressedHandler, AbilityInputID);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AFrogCharacter::AbilityInputBindingReleasedHandler, AbilityInputID);
		}
	}
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Grapple
	if (bIsGrapple) RedrawTongueLocation();
}

void AFrogCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFrogCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AFrogCharacter::AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(AbilityInputID));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AFrogCharacter::AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(AbilityInputID));
}

void AFrogCharacter::SetTongueVisibility(const bool Value) const
{
	if (IsValid(Tongue)) Tongue->SetVisibility(Value);
}

void AFrogCharacter::RedrawTongueLocation() const
{
	if (IsValid(Tongue)) Tongue->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePoint);
}

void AFrogCharacter::SpawnProjectile(const TSubclassOf<AProjectile> ActorClass, const FVector& Location, const FRotator& Rotation)
{
	const FVector FireDirection = FollowCamera->GetForwardVector(); 

	ServerSpawnPredictedProjectile(ActorClass, Location, Rotation, FireDirection);

	// Client prediction
	if (IsLocallyControlled() && GetWorld()->GetNetMode() == NM_Client)
	{
		SpawnPredictedProjectileInternal(ActorClass, Location, Rotation, FireDirection, true);
	}
}

void AFrogCharacter::ServerSpawnPredictedProjectile_Implementation(const TSubclassOf<AProjectile> ActorClass,
	const FVector& Location, const FRotator& Rotation, const FVector FireDirection)
{
	SpawnPredictedProjectileInternal(ActorClass, Location, Rotation, FireDirection, false);
	MulticastSpawnPredictedProjectile(ActorClass, Location, Rotation, FireDirection);
}

void AFrogCharacter::MulticastSpawnPredictedProjectile_Implementation(const TSubclassOf<AProjectile> ActorClass,
	const FVector& Location, const FRotator& Rotation, const FVector FireDirection)
{
	if (!HasAuthority() && !IsLocallyControlled())
	{
		SpawnPredictedProjectileInternal(ActorClass, Location, Rotation, FireDirection, true);
	}
}

void AFrogCharacter::SpawnPredictedProjectileInternal(const TSubclassOf<AProjectile>& ActorClass,
	const FVector& Location, const FRotator& Rotation, FVector FireDirection, const bool bIsVisualOnly)
{
	FActorSpawnParameters Params;
	Params.Owner = Params.Instigator = this;
	Params.CustomPreSpawnInitalization = [this, FireDirection, bIsVisualOnly](AActor* Actor)
	{
		if (const auto Projectile = Cast<AProjectile>(Actor))
		{
			Projectile->FireInDirection(FireDirection);
			Projectile->ToggleCollision(bIsVisualOnly);
		}
	};
    
	GetWorld()->SpawnActor<AProjectile>(ActorClass, Location, Rotation, Params);
}

