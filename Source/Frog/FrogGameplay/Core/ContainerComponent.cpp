#include "ContainerComponent.h"

#include "Item.h"
#include "ItemData.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"
#include "GameUI/Interactables/InventoryWidget.h"
#include "Net/UnrealNetwork.h"

UContainerComponent::UContainerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UContainerComponent, Inventory);
	DOREPLIFETIME(UContainerComponent, bIsInventoryWidgetVisible);
}

void UContainerComponent::Initialize(UItemData* InData, UInteractableWidgetComponent* InWidgetComponent, UMeshComponent* InMesh)
{
	Data = InData;
	InventoryWidgetComponent = InWidgetComponent;
	Mesh = InMesh;
}

void UContainerComponent::SetShowInventoryWidget(const bool bShow)
{
	if (!GetOwner()->HasAuthority()) return;

	if (bIsInventoryWidgetVisible != bShow)
	{
		bIsInventoryWidgetVisible = bShow;
		OnRep_IsInventoryWidgetVisible();
	}
}

void UContainerComponent::OnRep_IsInventoryWidgetVisible()
{
	if (InventoryWidgetComponent.IsValid())
	{
		InventoryWidgetComponent->SetVisibility(bIsInventoryWidgetVisible);
	}
}

void UContainerComponent::OnRep_Inventory()
{
	UpdateInventoryWidget();
}

void UContainerComponent::ClearInventory()
{
	if (!GetOwner()->HasAuthority()) return;
	
	for (const TObjectPtr<AItem>& Item : Inventory)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
    Inventory.Empty();
    OnRep_Inventory();
}

void UContainerComponent::RemoveNullsFromInventory()
{
	if (!GetOwner()->HasAuthority()) return;
	Inventory.Remove(nullptr);
}

bool UContainerComponent::TryAddToInventory(AItem* InteractableToAdd)
{
	if (!IsValid(GetOwner())) return false;
	if (!GetOwner()->HasAuthority()) return false;
    if (!Data.IsValid() || !IsValid(InteractableToAdd)) return false;
    if (IsFull()) return false;
    if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
	
	RemoveNullsFromInventory();
	
    Inventory.Add(InteractableToAdd);

    InteractableToAdd->DisableInteractable();
    
    const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
                                          EAttachmentRule::KeepWorld, false);
    InteractableToAdd->AttachToComponent(this, Rules); 
    
    OnRep_Inventory();
	OnAddedToInventory.Broadcast(InteractableToAdd);
    
    return true;
}

bool UContainerComponent::TryAddContainerContentsToInventory(AContainer* ContainerToAdd)
{
	if (!GetOwner()->HasAuthority()) return false;
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
	OnRep_Inventory();
    
	return true;
}

bool UContainerComponent::TryRemoveFromInventory(AItem* InteractableToRemove)
{
	if (!GetOwner()->HasAuthority()) return false;
    if (Inventory.Remove(InteractableToRemove) <= 0) return false;
	
	OnRep_Inventory();
	OnAddedToInventory.Broadcast(InteractableToRemove);
	
	return true;
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