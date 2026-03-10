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

bool UContainerComponent::TryAddToInventory(AItem* InteractableToAdd, UContainerComponent* SourceContainerComp)
{
	if (!IsValid(GetOwner())) return false;
	if (!GetOwner()->HasAuthority()) return false;
    if (!Data.IsValid() || !IsValid(InteractableToAdd)) return false;
    if (IsFull()) return false;
    if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
	
	RemoveNullsFromInventory();
	
    Inventory.Add(InteractableToAdd);
	
	if (IsValid(SourceContainerComp))
	{
		SourceContainerComp->TryRemoveFromInventory(InteractableToAdd);
	}

    InteractableToAdd->DisableInteractable();
    
    const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
                                          EAttachmentRule::KeepWorld, false);
    InteractableToAdd->AttachToComponent(this, Rules); 
    
    OnRep_Inventory();
	OnAddedToInventory.Broadcast(InteractableToAdd);
    
    return true;
}

bool UContainerComponent::TryAddContainerContentsToInventory(AContainer* SourceContainer)
{
	if (!GetOwner()->HasAuthority()) return false;
	if (!IsValid(SourceContainer)) return false;
	if (!Data.IsValid()) return false;

	UContainerComponent* SourceContainerComp = SourceContainer->GetContainerComponent();
	if (!IsValid(SourceContainerComp)) return false;
	
    RemoveNullsFromInventory();
	SourceContainerComp->RemoveNullsFromInventory();
	
	if (SourceContainerComp->IsEmpty()) return false;
	if (GetInventorySize() + SourceContainerComp->GetInventorySize() > Data->GetMaxCapacity()) return false;
    
	const TArray<TObjectPtr<AItem>>& InInventory = SourceContainerComp->GetInventory();
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
		TryAddToInventory(InInventory[i].Get(), SourceContainerComp);
	}
    
	return true;
}

bool UContainerComponent::TryRemoveFromInventory(AItem* InteractableToRemove)
{
	if (!GetOwner()->HasAuthority()) return false;
    if (Inventory.Remove(InteractableToRemove) <= 0) return false;
	
	OnRep_Inventory();
	OnRemovedFromInventory.Broadcast(InteractableToRemove);
	
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