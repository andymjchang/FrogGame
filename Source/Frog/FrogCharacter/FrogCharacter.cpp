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
#include "FrogMovementComponent.h"
#include "GAS/AbilitySet.h"
#include "GAS/UnitAttributeSet.h"
#include "GAS/FrogAbilitySystem.h"
#include "NametagWidgetComponent.h"
#include "Components/SphereComponent.h"
#include "FrogGameplay/Interactable.h"
#include "FrogGameplay/Item.h"
#include "FrogGameplay/Station.h"
#include "GameUI/FrogHUD.h"

//////////////////////////////////////////////////////////////////////////
// AFrogCharacter

AFrogCharacter::AFrogCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFrogMovementComponent>(CharacterMovementComponentName))
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
	
	// // Player hitbox
	// Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComponent"));
	// Hitbox->SetCollisionProfileName(TEXT("PlayerHitbox"));
	// Hitbox->SetupAttachment(RootComponent);
	InteractHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("InteractHitbox"));
	InteractHitbox->SetCollisionProfileName(TEXT("PickupItems"));
	InteractHitbox->SetupAttachment(RootComponent);

	// Setup Grapple
	// Tongue = CreateDefaultSubobject<UFrogTongue>(TEXT("TongueComponent"));
	// Tongue->SetupAttachment(GetRootComponent());

	// Frog Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UFrogAbilitySystem>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UFrogAttributeSet>(TEXT("AttributeSet"));
	
	// World space nametag
	NametagWidgetComponent = CreateDefaultSubobject<UNametagWidgetComponent>(TEXT("NametagWidgetComponent"));
	NametagWidgetComponent->SetupAttachment(RootComponent);

	// Held Interactable Attach Point
	InteractableAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractableAttachPoint"));
	InteractableAttachPoint->SetupAttachment(GetMesh());
}

void AFrogCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AFrogCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFrogCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup Widgets
	if (APlayerController* PC = GetController<APlayerController>())
	{
		
	}
	
	if (HasAuthority()) SetupAbilities();

	InteractHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapBegin);
	InteractHitbox->OnComponentEndOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapEnd);
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::Interact()
{
	const bool bCurrentlyHighlighting = CurrentInteractable.IsValid();
	const bool bHoldingInteractable = HeldInteractable.IsValid();
	
	// Interact with Moveable Interactable
	if (bCurrentlyHighlighting && CurrentInteractable->IsMoveable())
	{
		if (!bHoldingInteractable) // Pickup
		{
			CurrentInteractable->AttachToComponent(InteractableAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			HeldInteractable = CurrentInteractable;
			HeldInteractable->DisableInteractable();
		}
		// else // Place down
		// {
		// 	HeldInteractable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// 	HeldInteractable->EnableInteractable();
		// }
		return;
	}

	// Interact with Station
	if (AStation* Station = Cast<AStation>(CurrentInteractable.Get()))
	{
		if (bHoldingInteractable)
		{
			if (AItem* Item = Cast<AItem>(HeldInteractable))
			{
				if (Station->TryAddItem(Item))
				{
					// HeldInteractable->Destroy();
				}
			}
		}
		else
		{
			Station->OnInteract(this);
		}
	}
}

void AFrogCharacter::SetupAbilities()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet)) return;
	// Grant default abilities
	if (IsValid(AbilitySet))
	{
		InitialAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}

	// Apply initial stats
	if (IsValid(DefaultAttributes))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(DefaultAttributes->GetDefaultObject<UGameplayEffect>(), 0.f,
		                                                  AbilitySystemComponent->MakeEffectContext());
	}
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

		// GAS
		for (const auto [InputAction, AbilityTag] : AbilityInputBindings.Bindings)
		{
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AFrogCharacter::AbilityInputBindingPressedHandler, AbilityTag);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AFrogCharacter::AbilityInputBindingReleasedHandler, AbilityTag);
		}
	}
}

void AFrogCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInteractable* Interactable = Cast<AInteractable>(OtherActor))
	{
		OverlappingInteractables.Add(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::UpdateClosestInteractable()
{
	CurrentInteractable = nullptr;

	float BestDistanceSq = MAX_FLT;

	for (TWeakObjectPtr<AInteractable> I : OverlappingInteractables)
	{
		if (!I.IsValid()) continue;

		float DistSq = FVector::DistSquared(I->GetActorLocation(), GetActorLocation());
		if (DistSq < BestDistanceSq)
		{
			BestDistanceSq = DistSq;
			CurrentInteractable = I;
		}
	}

	// if (CurrentInteractable.IsValid() && GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
	// 		FString::Printf(TEXT("Current Interaction Target: %s"), *CurrentInteractable->GetName()));
	// }
}

void AFrogCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractable* Interactable = Cast<AInteractable>(OtherActor))
	{
		OverlappingInteractables.Remove(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (OverlappingInteractables.Num() > 1)
	{
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
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