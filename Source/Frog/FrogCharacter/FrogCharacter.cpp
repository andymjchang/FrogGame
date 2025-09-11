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
#include "Components/SphereComponent.h"
#include "Frog/GAS/AbilitySet.h"
#include "Frog/GAS/UnitAttributeSet.h"
#include "Frog/GAS/FrogAbilitySystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Unit/NametagWidgetComponent.h"
#include "Unit/ProjectileSpawnerComponent.h"

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

	// Player hitbox
	Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComponent"));
	Hitbox->SetCollisionProfileName(TEXT("PlayerHitbox"));
	Hitbox->SetupAttachment(RootComponent);

	// Setup Grapple
	Tongue = CreateDefaultSubobject<UFrogTongue>(TEXT("TongueComponent"));
	Tongue->SetupAttachment(GetRootComponent());

	// Frog Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UFrogAbilitySystem>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UFrogAttributeSet>(TEXT("AttributeSet"));

	// Projectile Spawner Component
	ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawnerComponent>(TEXT("ProjectileSpawner"));
	
	// World space healthbar/nametag
	HealthBarWidgetComponent = CreateDefaultSubobject<UNametagWidgetComponent>(TEXT("NametagWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFrogCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Grapple
	DOREPLIFETIME(AFrogCharacter, GrapplePoint);
	DOREPLIFETIME(AFrogCharacter, bIsGrapple);
	DOREPLIFETIME(AFrogCharacter, bFindEnemyUnderCrosshair);
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
		if (IsLocallyControlled() && FrogHUDClass)
		{
			FrogHUDWidget = CreateWidget<UFrogHUD>(PC, FrogHUDClass);
			if (FrogHUDWidget) FrogHUDWidget->AddToViewport();
		}
	}
	
	if (HasAuthority()) SetupAbilities();
}

UAbilitySystemComponent* AFrogCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFrogCharacter::SetupAbilities()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet)) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFrogAttributeSet::GetManaAttribute()).AddUObject(
		this, &AFrogCharacter::OnManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFrogAttributeSet::GetMaxManaAttribute()).AddUObject(
		this, &AFrogCharacter::OnMaxManaChanged);

	// Listen for when gameplay tags are applied
	GameplayTagAppliedHandle = AbilitySystemComponent->OnGameplayEffectAppliedDelegateToTarget.AddUObject(
		this, &AFrogCharacter::OnGameplayEffectApplied);

	// Listen for abilities that need point and click targeting
	TargetEnemyTagDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(FindEnemyUnderCrosshairGameplayTag).
		AddUObject(this, &AFrogCharacter::OnTargetEnemyTagChanged);

	// Grant default abilities
	if (IsValid(AbilitySet))
	{
		InitialAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent, FrogHUDWidget));
	}

	// Apply initial stats
	if (IsValid(DefaultAttributes))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(DefaultAttributes->GetDefaultObject<UGameplayEffect>(), 0.f,
		                                                  AbilitySystemComponent->MakeEffectContext());
	}
}

void AFrogCharacter::HandleDeath()
{
	if (IsValid(AbilitySystemComponent) && OnDeathEffect)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(OnDeathEffect, 1.0f, ContextHandle);
		
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	GetCharacterMovement()->MaxWalkSpeed = DownedSpeed;
	AbilitySystemComponent->CancelAllAbilities();
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void AFrogCharacter::SetHealth(const float NewHealth)
{
	
}

void AFrogCharacter::SetMaxHealth(const float NewMaxHealth)
{
	
}

UProjectileSpawnerComponent* AFrogCharacter::GetProjectileSpawnerComponent()
{
	return ProjectileSpawner;
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
		for (const auto [InputAction, AbilityTag] : AbilityInputBindings.Bindings)
		{
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AFrogCharacter::AbilityInputBindingPressedHandler, AbilityTag);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AFrogCharacter::AbilityInputBindingReleasedHandler, AbilityTag);
		}
	}
}

bool AFrogCharacter::CanJumpInternal_Implementation() const
{
	return JumpIsAllowedInternal();
}

void AFrogCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Grapple
	if (bIsGrapple) RedrawTongueLocation();

	// Auto Target Enemies
	if (bFindEnemyUnderCrosshair) FindEnemyUnderCrosshair();
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
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X * 0.5);
		AddControllerPitchInput(LookAxisVector.Y * 0.5);
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

void AFrogCharacter::OnManaChanged(const FOnAttributeChangeData& Data)
{
	if (FrogHUDWidget) FrogHUDWidget->UpdateMana(Data.NewValue);
}

void AFrogCharacter::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	if (FrogHUDWidget) FrogHUDWidget->UpdateMaxMana(Data.NewValue);
}

void AFrogCharacter::SetTongueVisibility(const bool Value)
{
	if (IsValid(Tongue)) Tongue->SetVisibility(Value);
}

void AFrogCharacter::RedrawTongueLocation()
{
	if (IsValid(Tongue)) Tongue->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePoint);
}

void AFrogCharacter::FindEnemyUnderCrosshair()
{
	const FVector TraceStart = FollowCamera->GetComponentLocation();
	const FVector CameraForward = FollowCamera->GetForwardVector();
	const float TraceDistance = FindEnemyUnderCrosshairTraceDistance;
	const FVector TraceEnd = TraceStart + (CameraForward * TraceDistance);

	FHitResult HitResult;
	
	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FindEnemyUnderCrosshairTraceRadius,
		FindEnemyUnderCrosshairObjectType,
		false,                     
		TArray<AActor*>(),          
		EDrawDebugTrace::None, 
		HitResult,
		true
	);
	
	if (bHit)
	{
		if (TargetEnemyActor != HitResult.GetActor())
		{
			TargetEnemyActor = HitResult.GetActor();
		}
	}
	else
	{
		TargetEnemyActor = nullptr;
	}
}

void AFrogCharacter::OnTargetEnemyTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bFindEnemyUnderCrosshair = NewCount > 0;
}

void AFrogCharacter::OnGameplayEffectApplied(UAbilitySystemComponent* InputAbilitySystem,
	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectSpec)
{
	FGameplayTagContainer GrantedTags;
	GameplayEffectSpec.GetAllAssetTags(GrantedTags);

	if (DamageTag.IsValid() && GrantedTags.HasTag(DamageTag))
	{
		FrogHUDWidget->PlayHitmarkerAnimation();
	}
	// AActor* TargetActor = AbilitySystemComponent->GetOwnerActor();
	// AActor* SourceActor = GameplayEffectSpec.GetContext().GetEffectCauser();
	//
	// if (SourceActor && SourceActor != TargetActor)
	// {
	// 	
	// }
}

FVector AFrogCharacter::GetFloorLocation()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, 1500.f);
	FVector FloorLocation;
	
	FHitResult HitResult;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
	);

	// Optional: Draw a debug line to visualize the trace
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1.0f, 0, 1.0f);
	
	if (bHit) FloorLocation = HitResult.Location;

	return FloorLocation;
}
