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
#include "FrogMovementComponent.h"

//////////////////////////////////////////////////////////////////////////
// AFrogCharacter

AFrogCharacter::AFrogCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFrogMovementComponent>(AFrogCharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Networking
	bReplicates = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 42.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1800.0f, 0.0f); 

	// Default CMC Values
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = false;
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = false;
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
	UnitAttributes = CreateDefaultSubobject<UUnitAttributeSet>(TEXT("UnitAttributes"));

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
	
	if (HasAuthority())
	{
		SetupAbilities();
	}
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::SetupAbilities()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(UnitAttributes)) return;
	if (IsValid(AbilitySet))
	{
		InitialAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}

	// Apply initial stats
	if (IsValid(InitialGameplayEffect))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
	}
	
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UUnitAttributeSet::GetHealthAttribute()).AddUObject(this, &AFrogCharacter::OnHealthAttributeChanged);
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Grapple
	if (bIsGrapple)
	{
		RedrawTongueLocation(DeltaSeconds);
	}
}

void AFrogCharacter::SetClientAuthoritativeMovement(const bool Value)
{
	GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = Value;
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = Value;
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

		for (const FAbilityInputToInputActionBinding binding : AbilityInputBindings.Bindings)
		{
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Started, this, &AFrogCharacter::AbilityInputBindingPressedHandler, binding.AbilityInputID);
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &AFrogCharacter::AbilityInputBindingReleasedHandler, binding.AbilityInputID);
		}
		// Bind Input actions to Enum entries
		// const FTopLevelAssetPath EnumName("/Script/Frog.EAbilityInputID");
		// FGameplayAbilityInputBinds Binds("Confirm", "Cancel", EnumName);
		// AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);
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

	if (IsValid(Controller))
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFrogCharacter::AbilityInputBindingPressedHandler(EAbilityInputID AbilityInputID)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(AbilityInputID));
}

void AFrogCharacter::AbilityInputBindingReleasedHandler(EAbilityInputID AbilityInputID)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(AbilityInputID));
}

void AFrogCharacter::Grapple(const FInputActionValue& Value)
{
	if (!TraceGrapplePoint() || !IsLocallyControlled()) return;
	
	bIsGrapple = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple));
	ServerGrapple(GrapplePoint);

}

void AFrogCharacter::ServerGrapple_Implementation(const FVector NewGrapplePoint)
{
	if (IsValid(Tongue))
	{
		Tongue->SetVisibility(true);
	}
	bIsGrapple = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Grapple));
	GrapplePoint = NewGrapplePoint;
	// SetClientAuthoritativeMovement(false);
}

void AFrogCharacter::StopGrapple(const FInputActionValue& Value)
{
	if (IsLocallyControlled())
	{
		// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "released");
		// SetClientAuthoritativeMovement(false);
		ServerStopGrapple();
	}
	bIsGrapple = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void AFrogCharacter::ServerStopGrapple_Implementation()
{
	bIsGrapple = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	// SetClientAuthoritativeMovement(false);
	// if (IsValid(Tongue)) Tongue->SetVisibility(false);
	// SetActorRelativeRotation(FRotator(0, 0, 0));
}

// Returns grapple point (world position)
bool AFrogCharacter::TraceGrapplePoint()
{
    if (!IsValid(FollowCamera)) return false;
    
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
    FHitResult CameraHitResult;
    bool bHit = GetWorld()->LineTraceSingleByChannel(
       CameraHitResult,
       CameraOffsetLocation,
       CameraLookPoint,
       ECC_Visibility,
       Params
    );
    
    // Debug line for camera trace
    FColor CameraTraceColor = bHit ? FColor::Red : FColor::Yellow;
    DrawDebugLine(
        GetWorld(),
        CameraOffsetLocation,
        bHit ? CameraHitResult.ImpactPoint : CameraLookPoint,
        CameraTraceColor,
        false, // Persistent lines
        2.0f,  // Lifetime in seconds
        0,     // Depth priority
        2.0f   // Line thickness
    );
    
    if (bHit)
    {
       CameraImpactPoint = CameraHitResult.ImpactPoint;
       
       // Debug sphere at camera impact point
       DrawDebugSphere(
           GetWorld(),
           CameraImpactPoint,
           10.0f,
           12,
           FColor::Red,
           false,
           2.0f
       );
    }
    else 
    { 
        return false; 
    }

    // Line trace from player to camera's impact point
    FVector PlayerToCameraPoint = CameraImpactPoint - GetActorLocation();
    PlayerToCameraPoint.Normalize();
    FVector PlayerLookPoint = GetActorLocation() + PlayerToCameraPoint * CameraGrappleLength;
    FHitResult PlayerHitResult;
    bool bHit2 = GetWorld()->LineTraceSingleByChannel(
       PlayerHitResult,
       GetActorLocation(),
       PlayerLookPoint,
       ECC_Visibility,
       Params
    );
    
    // Debug line for player trace
    FColor PlayerTraceColor = bHit2 ? FColor::Green : FColor::Orange;
    DrawDebugLine(
        GetWorld(),
        GetActorLocation(),
        bHit2 ? PlayerHitResult.ImpactPoint : PlayerLookPoint,
        PlayerTraceColor,
        false,
        2.0f,
        0,
        2.0f
    );
    
    if (bHit2)
    {
       GrapplePoint = PlayerHitResult.ImpactPoint;
       
       // Debug sphere at grapple point
       DrawDebugSphere(
           GetWorld(),
           GrapplePoint,
           15.0f,
           12,
           FColor::Green,
           false,
           2.0f
       );
    }
    
    return bHit2;
}

void AFrogCharacter::SetTongueVisibility(const bool Value)
{
	if (IsValid(Tongue)) Tongue->SetVisibility(Value);
}

void AFrogCharacter::RedrawTongueLocation(float DeltaSeconds)
{
	if (IsValid(Tongue))
	{
		// Tongue->SetEndLocationReplicated(GetActorTransform().InverseTransformPosition(GrapplePoint));
		Tongue->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePoint);
	}
}

