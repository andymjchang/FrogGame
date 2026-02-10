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
#include "FrogGameplay/Item.h"
#include "FrogGameplay/ItemData.h"

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
	
	// // Player hitboxs
	InteractHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("InteractHitbox"));
	InteractHitbox->SetCollisionProfileName(TEXT("Interact"));
	InteractHitbox->SetupAttachment(RootComponent);
	WorkHitbox =  CreateDefaultSubobject<USphereComponent>(TEXT("WorkHitbox"));
	WorkHitbox->SetCollisionProfileName(TEXT("Work"));
	WorkHitbox->SetupAttachment(RootComponent);

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
	
	if (HasAuthority()) SetupAbilities();

	InteractHitbox->OnComponentBeginOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapBegin);
	InteractHitbox->OnComponentEndOverlap.AddDynamic(this, &AFrogCharacter::OnOverlapEnd);
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::Interact() {
    AItem* OtherInteractable = CurrentInteractable.Get();
    if (!IsValid(OtherInteractable))
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Interact: CurrentInteractable is invalid"), GetWorld()->GetTimeSeconds());
        return;
    }
    
    AItem* OtherOffer = CurrentInteractable->GetOfferedInteractable();
    if (!IsValid(OtherOffer))
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Interact: OtherOffer is invalid for %s"), GetWorld()->GetTimeSeconds(), *OtherInteractable->GetName());
        return;
    }
    
    // If holding an item
    if (HeldInteractable.IsValid())
    {
    	UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Holding item %s, attempting to add %s to it"), GetWorld()->GetTimeSeconds(), *HeldInteractable->GetName(), *OtherOffer->GetName());
    	
        // Try adding Other to Held Item
        if (HeldInteractable->TryAddToInventory(OtherOffer))
        {
            // Remove from source's inventory
            OtherInteractable->TryRemoveFromInventory(OtherOffer);
            
            UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Successfully added other's offer to held item"), GetWorld()->GetTimeSeconds());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Failed to add to held item, trying reverse add"), GetWorld()->GetTimeSeconds());
        	
        	if (OtherOffer->TryAddToInventory(HeldInteractable.Get()))
            {
            	UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Successfully added held item to other, clearing held item"), GetWorld()->GetTimeSeconds());
            	HeldInteractable = nullptr;
            }
        	else if (HeldInteractable.Get()->GetIsContainer())
            {
            	UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Failed to add held item to other, try adding as container"), GetWorld()->GetTimeSeconds());
            	if (OtherOffer->TryAddContainerToInventory(HeldInteractable.Get()))
            	{
            		UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Successfully added container contents to other"), GetWorld()->GetTimeSeconds());
            	}
            	else
            	{
            		UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Failed to add held item as a container"), GetWorld()->GetTimeSeconds());
            	}
            }
        }
    }
    else // If not holding an item
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Not holding item, attempting to add %s to player"), GetWorld()->GetTimeSeconds(), *OtherOffer->GetName());
        
        // Try adding Other's Offer to Player
        if (TryAddInteractableToPlayer(OtherOffer))
        {
            // Remove from source's inventory
            OtherInteractable->TryRemoveFromInventory(OtherOffer);
            
            UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Successfully added other's offer to player"), GetWorld()->GetTimeSeconds());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Failed to add offer, trying to add %s directly to player"), GetWorld()->GetTimeSeconds(), *OtherInteractable->GetName());
            
            // Try adding Other to Player
            if (TryAddInteractableToPlayer(OtherInteractable))
            {
                UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Successfully added interactable directly to player"), GetWorld()->GetTimeSeconds());
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("[%f] Interact: Failed to add interactable directly to player"), GetWorld()->GetTimeSeconds());
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

bool AFrogCharacter::TryAddInteractableToPlayer(AItem* InteractableToAdd)
{
    if (!IsValid(InteractableToAdd))
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Player: InteractableToAdd is not valid"), GetWorld()->GetTimeSeconds());
        return false;
    }
    
    if (!IsValid(InteractableToAdd->GetData()))
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Player: InteractableToAdd has no valid Data"), GetWorld()->GetTimeSeconds());
        return false;
    }
    
    if (!InteractableToAdd->GetData()->GetIsMoveable())
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Player: Interactable is not moveable"), GetWorld()->GetTimeSeconds());
        return false;
    }

    if (HeldInteractable.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("[%f] Player: Already holding an item"), GetWorld()->GetTimeSeconds());
        return false;
    }
    
    if (InteractableToAdd->HasMatchingInteractableTag(AcceptedTags))
    {
        const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
        HeldInteractable = InteractableToAdd;
        InteractableToAdd->DisableInteractable();
        InteractableToAdd->AttachToComponent(InteractableAttachPoint, Rules);
       
        UE_LOG(LogTemp, Log, TEXT("[%f] Player: Successfully picked up interactable"), GetWorld()->GetTimeSeconds());
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("[%f] Player: Interactable tags not compatible with player"), GetWorld()->GetTimeSeconds());

    return false;
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
	if (AItem* Interactable = Cast<AItem>(OtherActor))
	{
		OverlappingInteractables.Add(Interactable);
		UpdateClosestInteractable();
	}
}

void AFrogCharacter::UpdateClosestInteractable()
{
	CurrentInteractable = nullptr;

	float BestDistanceSq = MAX_FLT;

	for (TWeakObjectPtr<AItem> I : OverlappingInteractables)
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
	if (AItem* Interactable = Cast<AItem>(OtherActor))
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