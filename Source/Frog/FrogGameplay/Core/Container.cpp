// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"

#include "ContainerComponent.h"
#include "Frog.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ContainerComponent = CreateDefaultSubobject<UContainerComponent>(TEXT("ContainerComponent"));
	ContainerComponent->SetupAttachment(GetRootComponent());
	
	InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
}

TScriptInterface<IItemInterface> AContainer::SpawnItem(const TSubclassOf<AActor> ClassToSpawn)
{
	if (!HasAuthority()) return nullptr;

	UWorld* World = GetWorld();
	if (!World) return nullptr;
    
	if (!IsValid(ClassToSpawn) || !IsValid(ContainerComponent)) return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (AActor* NewActor = World->SpawnActor<AActor>(ClassToSpawn, ContainerComponent->GetComponentTransform(), SpawnParams))
	{
		return NewActor;
	}
	
	return nullptr;
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ContainerComponent))
	{
		ContainerComponent->OnAddedToInventory.AddDynamic(this, &AContainer::HandleAddedToInventory);
		ContainerComponent->OnRemovedFromInventory.AddDynamic(this, &AContainer::HandleRemovedFromInventory);
		ContainerComponent->Initialize(Data, InventoryWidgetComponent, InteractableMesh.Get());
	}
}

void AContainer::Destroyed()
{
	if (HasAuthority() && IsValid(ContainerComponent))
	{
		ContainerComponent->ClearInventory();
	}

	Super::Destroyed();
}

void AContainer::HandleAddedToInventory(const TScriptInterface<IItemInterface>& Interactable)
{

}

void AContainer::HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable)
{
	OfferedInteractable = this;
}



