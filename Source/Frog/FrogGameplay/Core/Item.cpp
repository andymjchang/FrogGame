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
	// NetDormancy = DORM_Initial;
	
	// Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	
	// Interact Hitbox
	InteractHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractHitBox"));
	InteractHitBox->SetupAttachment(RootComponent);
	InteractHitBox->SetCollisionProfileName(TEXT("InteractListen"));
	InteractHitBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	InteractHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 128.0f));
	InteractHitBox->SetIsReplicated(true);
	
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

bool AItem::HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const
{
	if (AcceptedTags.IsEmpty()) return false;
	if (!IsValid(Data)) return false;

	const FGameplayTagContainer& OwnedTags = Data->GetOwnedTags();
	for (const FGameplayTag& OwnedTag : OwnedTags)
	{
		if (OwnedTag.MatchesAny(AcceptedTags))
		{
			return true;
		}
	}

	return false;
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

void AItem::StartInteract()
{
}

// void AItem::EnableInteractable()
// {
// 	FlushNetDormancy();
// 	// NetDormancy = DORM_Awake;
// }

void AItem::DisableHitbox()
{
	if (!HasAuthority()) return;
	
	bIsHitboxEnabled = false;
	OnRep_bIsHitboxEnabled();
	// ForceNetUpdate();
	// NetDormancy = DORM_DormantAll;
}