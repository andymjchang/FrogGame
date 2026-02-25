#include "ContainerComponent.h"

#include "Item.h"
#include "ItemData.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"

UContainerComponent::UContainerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UContainerComponent::Initialize(UItemData* InData, UInteractableWidgetComponent* InWidgetComponent)
{
	Data = InData;
	InventoryWidgetComponent = InWidgetComponent;
}

void UContainerComponent::SetShowInventoryWidget(const bool bShow)
{
	if (InventoryWidgetComponent.IsValid())
	{
		InventoryWidgetComponent->SetVisibility(bShow);
	}
}

void UContainerComponent::ClearInventory()
{
    Inventory.Empty();
    UpdateInventoryWidget();
}

void UContainerComponent::RemoveNullsFromInventory()
{
	Inventory.Remove(nullptr);
}

bool UContainerComponent::TryAddToInventory(AItem* InteractableToAdd)
{
    if (!Data.IsValid() || !IsValid(InteractableToAdd)) return false;
    if (IsFull()) return false;
    if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
	
	RemoveNullsFromInventory();
	
    Inventory.Add(InteractableToAdd);

    InteractableToAdd->DisableInteractable();
    const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
    InteractableToAdd->AttachToComponent(this, Rules); 
    
    UpdateInventoryWidget();
    OnAddedToInventory.ExecuteIfBound(InteractableToAdd);
    
    return true;
}

bool UContainerComponent::TryAddContainerContentsToInventory(AContainer* ContainerToAdd)
{
	if (!IsValid(ContainerToAdd)) return false;
	if (!Data.IsValid()) return false;

	UContainerComponent* ContainerCompToAdd = ContainerToAdd->GetContainerComponent();
	if (!IsValid(ContainerCompToAdd)) return false;
	
    RemoveNullsFromInventory();
	ContainerCompToAdd->RemoveNullsFromInventory();
	
	if (ContainerCompToAdd->IsEmpty()) return false;
	if (GetInventorySize() + ContainerCompToAdd->GetInventorySize() > Data->GetMaxCapacity()) return false;
    
	const TArray<TObjectPtr<AItem>>& InInventory = ContainerCompToAdd->GetInventory();
	// All contents of the source must be compatible, no partial transfer
	for (const TObjectPtr<AItem>& InventoryItem : InInventory)
	{
		if (IsValid(InventoryItem) && !InventoryItem->HasMatchingInteractableTag(Data->GetAcceptedTags()))
		{
			return false;
		}
	}
    
	for (int32 i = InInventory.Num() - 1; i >= 0; --i)
	{
		TryAddToInventory(InInventory[i].Get());
	}
    
	ContainerCompToAdd->ClearInventory();
	UpdateInventoryWidget();
    
	return true;
}

bool UContainerComponent::TryRemoveFromInventory(AItem* InteractableToRemove)
{
    if (Inventory.Remove(InteractableToRemove) > 0)
    {
		UpdateInventoryWidget();
    	if (IsValid(InteractableToRemove))
    	{
    		OnRemovedFromInventory.ExecuteIfBound(InteractableToRemove);
    	}
		return true;
    }

    return false;
}

void UContainerComponent::UpdateInventoryWidget()
{
	
    if (InventoryWidgetComponent.IsValid() && IsValid(InventoryWidgetComponent->GetWidget()))
    {
	    if (UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(InventoryWidgetComponent->GetWidget()))
	    {
		    InventoryWidget->UpdateInventoryWidget(Inventory);
	    }
    }
}