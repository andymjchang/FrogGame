// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Frog.h"
#include "GameplayTagContainer.h"
#include "ItemData.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	NetDormancy = DORM_DormantAll;
	
	// Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(150.f, 150.f, 150.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	
	// Static Mesh Component
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
	InteractableMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AItem, bIsHitboxEnabled);
}

FVector AItem::GetInteractableLocation() const
{
	return GetRootComponent()->GetComponentLocation();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	OfferedInteractable = this;
}

void AItem::EnableHitbox()
{
	if (!HasAuthority()) return;

	bIsHitboxEnabled = true;
	OnRep_bIsHitboxEnabled();
	SetItemDormancy(false);
}

void AItem::DisableHitbox()
{
	if (!HasAuthority()) return;
	
	bIsHitboxEnabled = false;
	OnRep_bIsHitboxEnabled();
}

void AItem::OnRep_bIsHitboxEnabled()
{
	if (IsValid(InteractHitBox))
	{
		// FLOG(TEXT("ID: %d bIsHitboxEnabled: %d"), GetWorld()->GetNetMode(), bIsHitboxEnabled);
		if (bIsHitboxEnabled)
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			InteractHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AItem::SetItemDormancy(const bool bDormant)
{
	if (!HasAuthority()) return;

	if (bDormant)
	{
		NetDormancy = DORM_DormantAll;
		ForceNetUpdate();
	}
	else
	{
		NetDormancy = DORM_Awake;
		ForceNetUpdate();
	}
}