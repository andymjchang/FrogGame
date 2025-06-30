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
#include "FrogAbilitySystem.h"
#include "FrogAbilityJump.h"
#include "FrogTongue.h"
#include "Net/UnrealNetwork.h"
#include "UObject/TopLevelAssetPath.h"

//////////////////////////////////////////////////////////////////////////
// AFrogCharacter

AFrogCharacter::AFrogCharacter()
{
	// Networking
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 42.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1800.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->BrakingFriction = 8.f;
	GetCharacterMovement()->MaxFlySpeed = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Setup Grapple
	Tongue = CreateDefaultSubobject<UFrogTongue>(TEXT("TongueComponent"));
	Tongue->SetupAttachment(GetRootComponent());
	CameraGrappleLength = 3000.f;
	GrappleStrength = 250000.f;

	// Frog Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UFrogAbilitySystem>(TEXT("AbilitySystem"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFrogCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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

	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
		{
			if (Ability)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(EAbilityInputID::Confirm), this));
			}
		}
	}
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Grapple
	if (bIsGrapple)
	{
		ApplyGrappleForce(DeltaSeconds);
	}
}

void AFrogCharacter::ApplyGrappleForce(float DeltaSeconds)
{
	FVector GrappleDirection = (GrapplePoint - GetActorLocation()).GetSafeNormal();
	SetActorRotation(GrappleDirection.Rotation());
	SetActorRelativeRotation(GetActorRotation() + FRotator(-30, 0, 0));
	GetCharacterMovement()->AddForce(GrappleDirection * GrappleStrength);
	Tongue->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePoint);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFrogCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
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
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrogCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrogCharacter::Look);
		// Grapple
		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &AFrogCharacter::Grapple);
		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AFrogCharacter::StopGrapple);

		// Bind Input actions to Enum entries
		const FTopLevelAssetPath EnumName("/Script/Frog.EAbilityInputID");
		FGameplayAbilityInputBinds Binds("Confirm", "Cancel", EnumName);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);
	}
}

void AFrogCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFrogCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFrogCharacter::Grapple(const FInputActionValue& Value)
{
	HandleGrapple(Value);
}

void AFrogCharacter::HandleGrapple_Implementation(const FInputActionValue& Value)
{
	if (bool ValidHit = GetGrapplePoint())
	{
		bIsGrapple = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying); 
		if (Tongue)
		{
			Tongue->SetVisibility(true);
			// Tongue->EndLocation = FVector(0, 0, 0);
		}
	}
}

void AFrogCharacter::StopGrapple(const FInputActionValue& Value)
{
	HandleStopGrapple(Value);
}

void AFrogCharacter::HandleStopGrapple_Implementation(const FInputActionValue& Value)
{
	bIsGrapple = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	if (Tongue) Tongue->SetVisibility(false);
	SetActorRelativeRotation(FRotator(0, 0, 0));
}

// Returns grapple point (world position)
bool AFrogCharacter::GetGrapplePoint()
{
	if (!FollowCamera) return false;
	
	// Get camera look at point
	FVector CameraWorldLocation = FollowCamera->GetComponentLocation();
	FVector CameraForward = FollowCamera->GetForwardVector();
	FVector CameraLookPoint = CameraWorldLocation + CameraForward * CameraGrappleLength;
	float CameraDistance = (FollowCamera->GetComponentLocation() - GetActorLocation()).Size();
	FVector CameraOffsetLocation = CameraWorldLocation + CameraForward * CameraDistance;
	FVector CameraImpactPoint;
	
	// Line trace from camera to look point
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraOffsetLocation,
		CameraLookPoint,
		ECC_Visibility, // Trace channel
		Params
	);
	if (bHit)
	{
		CameraImpactPoint = HitResult.ImpactPoint;
	}
	else { return false; }

	// Line trace from player to impact point
	FVector PlayerToCameraPoint = CameraImpactPoint - GetActorLocation();
	PlayerToCameraPoint.Normalize();
	FVector PlayerLookPoint = GetActorLocation() + PlayerToCameraPoint * CameraGrappleLength;
	FHitResult HitResult2;
	bool bHit2 = GetWorld()->LineTraceSingleByChannel(
		HitResult2,
		GetActorLocation(),
		PlayerLookPoint,
		ECC_Visibility, // Trace channel
		Params
	);
	if (bHit2)
	{
		GrapplePoint = HitResult.ImpactPoint;
	}
	return bHit2;
}

