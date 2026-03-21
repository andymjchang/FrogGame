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
#include "Frog.h"
#include "InputActionValue.h"
#include "FrogMovementComponent.h"
#include "GAS/AbilitySet.h"
#include "GAS/UnitAttributeSet.h"
#include "GAS/FrogAbilitySystem.h"
#include "NametagWidgetComponent.h"
#include "Components/SphereComponent.h"
#include "FrogGameplay/Core/Container.h"
#include "FrogGameplay/Core/ContainerComponent.h"
#include "FrogGameplay/Stations/WorkStation.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AFrogCharacter::AFrogCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFrogMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	GetCapsuleComponent()->InitCapsuleSize(145.0f, 145.0f);

	// Character Movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1800.0f, 0.0f); 
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
	
	// // Player hitboxs
	InteractHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("InteractHitbox"));
	InteractHitbox->SetCollisionProfileName(TEXT("Interact"));
	InteractHitbox->SetupAttachment(RootComponent);

	// World space nametag
	NametagWidgetComponent = CreateDefaultSubobject<UNametagWidgetComponent>(TEXT("NametagWidgetComponent"));
	NametagWidgetComponent->SetupAttachment(GetMesh());

	// Inventory
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetMesh());
	
	// Inventory Widget
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(GetMesh());
	InventoryWidgetComponent->SetRelativeLocation(FVector(0.f, 0.0f, 600.0f));
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
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
	
	if (IsValid(InteractHitbox))
	{
		InteractHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapBegin);
		InteractHitbox->OnComponentEndOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapEnd);
	}

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->Initialize(PlayerItemData, InventoryWidgetComponent, GetMesh());
		ContainerComponent->OnAddedToInventory.AddDynamic(this, &AFrogCharacter::HandleAddedToInventory);
	}
	
	if (IsValid(ContainerComponent)) ContainerComponent->SetShowInventoryWidget(false);
}

void AFrogCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		SetupAbilities();
	}
}

void AFrogCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
	if (IInteractableInterface* Interactable = Cast<IInteractableInterface>(OtherActor))
	{
		OverlappingInteractableArray.Add(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IInteractableInterface* Interactable = Cast<IInteractableInterface>(OtherActor))
	{
		OverlappingInteractableArray.Remove(Interactable);
		if (IsLocallyControlled())
		{
			Interactable->StopHighlight();
		}
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::UpdateClosestInteractable()
{
	IInteractableInterface* OldClosest = ClosestInteractable.Get();
	ClosestInteractable = nullptr;
	
	float BestDistanceSq = MAX_FLT;
	for (TWeakInterfacePtr Interactable : OverlappingInteractableArray)
	{
		if (!Interactable.IsValid()) continue;
    
		float DistSq = MAX_FLT;

		if (AActor* InteractableActor = Cast<AActor>(Interactable.GetObject()))
		{
			FVector Origin, Extent;
			// 'true' ensures we only get bounds for components that have collision enabled
			InteractableActor->GetActorBounds(true, Origin, Extent); 
			FBox ActorBounds = FBox::BuildAABB(Origin, Extent);
        
			DistSq = ActorBounds.ComputeSquaredDistanceToPoint(InteractHitbox->GetComponentLocation());
		}
		else
		{
			// Fallback just in case the interactable isn't an Actor
			DistSq = FVector::DistSquared(Interactable->GetInteractableLocation(), InteractHitbox->GetComponentLocation());
		}

		if (DistSq < BestDistanceSq)
		{
			BestDistanceSq = DistSq;
			ClosestInteractable = Interactable;
		}
	}

	if (OldClosest != ClosestInteractable.Get())
	{
		if (OldClosest)
		{
			if (HasAuthority())
			{
				OldClosest->StopInteract();
				if (AWorkStation* WorkStation = Cast<AWorkStation>(OldClosest))
				{
					WorkStation->StopWork(GetPlayerState());
				}
			}

			if (IsLocallyControlled())
			{
				OldClosest->StopHighlight();
			}
		}

		if (ClosestInteractable.IsValid() && IsLocallyControlled())
		{
			ClosestInteractable->StartHighlight(HighlightMaterial);
		}
	}
}

void AFrogCharacter::HandleAddedToInventory(AItem* Item)
{
	if (!HasAuthority()) return;
	
	// Set inventory visibility
	if (IsValid(ContainerComponent))
	{
		const bool IsContainer = IsValid(Cast<AContainer>(Item));
		ContainerComponent->SetShowInventoryWidget(!IsContainer);
	}
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (OverlappingInteractableArray.Num() > 1)
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