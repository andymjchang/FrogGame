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
#include "FrogGameplay/Container.h"
#include "FrogGameplay/ContainerComponent.h"
#include "FrogGameplay/Interactable.h"

AFrogCharacter::AFrogCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFrogMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
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
	
	// // Player hitboxs
	InteractHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("InteractHitbox"));
	InteractHitbox->SetCollisionProfileName(TEXT("Interact"));
	InteractHitbox->SetupAttachment(RootComponent);
	WorkHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("WorkHitbox"));
	WorkHitbox->SetCollisionProfileName(TEXT("Work"));
	WorkHitbox->SetupAttachment(RootComponent);

	// World space nametag
	NametagWidgetComponent = CreateDefaultSubobject<UNametagWidgetComponent>(TEXT("NametagWidgetComponent"));
	NametagWidgetComponent->SetupAttachment(RootComponent);

	// Inventory
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetMesh());
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
	
	if (HasAuthority()) SetupAbilities();

	if (IsValid(InteractHitbox))
	{
		InteractHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapBegin);
		InteractHitbox->OnComponentEndOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapEnd);
	}

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->Initialize(ContainerData);
		ContainerComponent->OnAddedToInventory.BindDynamic(this, &AFrogCharacter::HandleAddedToInventory);
	}
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::Interact()
{
	if (!IsValid(ContainerComponent)) return;

	AInteractable* OtherInteractable = ClosestInteractable.Get();
	if (!IsValid(OtherInteractable)) return;
	AInteractable* OtherOffer = OtherInteractable->GetOfferedInteractable();
	if (!IsValid(OtherOffer)) return;

	UContainerComponent* OtherContainerComp = nullptr;
	if (AContainer* OtherContainer = Cast<AContainer>(OtherInteractable))
	{
		OtherContainerComp = OtherContainer->GetContainerComponent();
	}
	UContainerComponent* OtherOfferAsContainerComp = nullptr;
	if (AContainer* OtherOfferAsContainer = Cast<AContainer>(OtherOffer))
	{
		OtherOfferAsContainerComp = OtherOfferAsContainer->GetContainerComponent();
	}

	// if (IsValid(OtherOfferAsContainerComp) && IsValid(GEngine))
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
	// 		FString::Printf(TEXT("Current Interaction Target: %s"), *OtherInteractable->GetName()));
	// }

	AInteractable* HeldInteractable = ContainerComponent->GetFirstItem();
	if (IsValid(HeldInteractable))
	{
		FLOG(TEXT("Trying interact as item..."));

		if (IsValid(OtherOfferAsContainerComp) && OtherOfferAsContainerComp->TryAddToInventory(HeldInteractable))
		{
			ContainerComponent->TryRemoveFromInventory(HeldInteractable);
		}
		else if (AContainer* HeldContainer = Cast<AContainer>(HeldInteractable))
		{
			FLOG(TEXT("Try interact as container..."));
			
			UContainerComponent* HeldContainerComp = HeldContainer->GetContainerComponent();
			if (IsValid(HeldContainerComp))
			{
				if (IsValid(OtherContainerComp) && HeldContainerComp->TryAddToInventory(OtherOffer))
				{
					OtherContainerComp->TryRemoveFromInventory(OtherOffer);
				}
				else if (IsValid(OtherOfferAsContainerComp))
				{
					OtherOfferAsContainerComp->TryAddContainerContentsToInventory(HeldContainer);
				}
			}
		}
	}
	else 
	{
		FLOG(TEXT("Trying interact as player"));
       
		if (ContainerComponent->TryAddToInventory(OtherOffer))
		{
			if (IsValid(OtherContainerComp))
			{
				OtherContainerComp->TryRemoveFromInventory(OtherOffer);
			}
		}
	}
}

void AFrogCharacter::Work()
{
	WorkHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFrogCharacter::StopWork()
{
	WorkHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		OverlappingInteractableArray.Add(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractable* Interactable = Cast<AInteractable>(OtherActor))
	{
		OverlappingInteractableArray.Remove(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::UpdateClosestInteractable()
{
	ClosestInteractable = nullptr;

	float BestDistanceSq = MAX_FLT;

	for (TWeakObjectPtr<AInteractable> I : OverlappingInteractableArray)
	{
		if (!I.IsValid()) continue;

		float DistSq = FVector::DistSquared(I->GetActorLocation(), GetActorLocation());
		if (DistSq < BestDistanceSq)
		{
			BestDistanceSq = DistSq;
			ClosestInteractable = I;
		}
	}
}

void AFrogCharacter::HandleAddedToInventory(class AInteractable* Item)
{
	const bool ShowInventory = (IsValid(Item) && !Cast<AContainer>(Item));
	ContainerComponent->SetShowInventoryWidget(ShowInventory);
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