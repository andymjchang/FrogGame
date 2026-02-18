#include "ContainerComponent.h"

#include "Frog.h"
#include "Interactable.h"
#include "InteractableData.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"

UContainerComponent::UContainerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPoint"));
    AttachPoint->SetupAttachment(this);
    
    InventoryWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("InventoryWidgetComponent"));
    InventoryWidgetComponent->SetupAttachment(this);
    InventoryWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
    InventoryWidgetComponent->SetDrawSize(FIntPoint(100, 100));
}

void UContainerComponent::Initialize(UInteractableData* InData)
{
	Data = InData;
}

void UContainerComponent::OnRegister()
{
	Super::OnRegister();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (AttachPoint)
	{
		// Force the attachment to the current instance to avoid Template Mismatch
		// This fixes the case where the cloned component still points to the template parent
		AttachPoint->SetupAttachment(this);
		
		if (!AttachPoint->IsRegistered())
		{
			AttachPoint->RegisterComponent();
		}
	}

	if (InventoryWidgetComponent)
	{
		InventoryWidgetComponent->SetupAttachment(this);
		
		if (!InventoryWidgetComponent->IsRegistered())
		{
			InventoryWidgetComponent->RegisterComponent();
		}
	}
}

void UContainerComponent::OnUnregister()
{
	if (AttachPoint && AttachPoint->IsRegistered())
	{
		AttachPoint->UnregisterComponent();
	}

	if (InventoryWidgetComponent && InventoryWidgetComponent->IsRegistered())
	{
		InventoryWidgetComponent->UnregisterComponent();
	}

	Super::OnUnregister();
}

void UContainerComponent::ClearInventory()
{
    Inventory.Empty();
    UpdateInventoryWidget();
}

bool UContainerComponent::TryAddToInventory(AInteractable* InteractableToAdd)
{
    if (!Data.IsValid() || !IsValid(InteractableToAdd)) return false;
    if (GetInventorySize() >= Data->GetMaxCapacity()) return false;
    if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
    
    Inventory.Add(InteractableToAdd);

    InteractableToAdd->DisableInteractable();
    const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
    InteractableToAdd->AttachToComponent(AttachPoint, Rules); 
    
    UpdateInventoryWidget();
    OnAddedToInventory.ExecuteIfBound(InteractableToAdd);
    
    return true;
}

bool UContainerComponent::TryAddContainerContentsToInventory(AContainer* ContainerToAdd)
{
    if (!IsValid(ContainerToAdd)) return false;
    if (!Data.IsValid()) return false;

    UContainerComponent* OtherContainer = ContainerToAdd->GetContainerComponent();
    if (!IsValid(OtherContainer)) return false;
    
    if (OtherContainer->GetInventorySize() <= 0) return false;
    if (GetInventorySize() + OtherContainer->GetInventorySize() > Data->GetMaxCapacity()) return false;
    
    const TArray<AInteractable*>& InInventory = OtherContainer->GetInventory();

    for (const AInteractable* InventoryIndex : InInventory)
    {
       if (!InventoryIndex->HasMatchingInteractableTag(Data->GetAcceptedTags()))
       {
          return false;
       }
    }
    
    for (int32 i = InInventory.Num() - 1; i >= 0; --i)
    {
       TryAddToInventory(InInventory[i]);
    }
    
    OtherContainer->ClearInventory();
    UpdateInventoryWidget();
    
    return true;
}

bool UContainerComponent::TryRemoveFromInventory(AInteractable* InteractableToRemove)
{
    if (!IsValid(InteractableToRemove)) return false;
    
    if (Inventory.Remove(InteractableToRemove) > 0)
    {
       UpdateInventoryWidget();
       OnRemovedFromInventory.ExecuteIfBound(InteractableToRemove);
       return true;
    }
    return false;
}

void UContainerComponent::UpdateInventoryWidget()
{
    if (UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(InventoryWidgetComponent->GetWidget()))
    {
       InventoryWidget->UpdateInventoryWidget(Inventory);
    }
}