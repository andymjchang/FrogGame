#include "ContainerComponent.h"

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
	DOREPLIFETIME(UContainerComponent, bAllowAdd);
	DOREPLIFETIME(UContainerComponent, bAllowRemove);
}

void UContainerComponent::Initialize(UItemData* InData, UInteractableWidgetComponent* InWidgetComponent, UMeshComponent* InMesh)
{
	Data = InData;
	InventoryWidgetComponent = InWidgetComponent;
	Mesh = InMesh;
}

void UContainerComponent::SetShowInventoryWidget(const bool bShow)
{
	// if (!GetOwner()->HasAuthority()) return;
	bIsInventoryWidgetVisible = bShow;
	OnRep_IsInventoryWidgetVisible();
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
	
	if (GetAttachParentActor())
	{
		GetAttachParentActor()->ForceNetUpdate();	
	}
	
	for (const TScriptInterface Item : Inventory)
	{
		AActor* ItemActor = Cast<AActor>(Item.GetObject());
		if (IsValid(ItemActor))
		{
			ItemActor->Destroy();
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

bool UContainerComponent::TryAddToInventory(const TScriptInterface<IItemInterface>& InteractableToAdd, UContainerComponent* SourceContainerComp)
{
	if (!IsValid(GetOwner())) return false;
	if (!GetOwner()->HasAuthority()) return false;
    if (!Data.IsValid() || !InteractableToAdd) return false;

	AActor* ItemActor = Cast<AActor>(InteractableToAdd.GetObject());
	if (!IsValid(ItemActor)) return false;
	
	if (IsFull() || !bAllowAdd) return false;
    if (!InteractableToAdd->HasMatchingInteractableTag(Data->GetAcceptedTags())) return false;
	
	if (GetAttachParentActor())
	{
		GetAttachParentActor()->ForceNetUpdate();	
	}
	
    // InteractableToAdd->SetItemDormancy(true);
	RemoveNullsFromInventory();
	
	if (IsValid(SourceContainerComp))
	{
		if (!SourceContainerComp->GetAllowRemove()) return false;
		SourceContainerComp->TryRemoveFromInventory(InteractableToAdd);
	}
    Inventory.Add(InteractableToAdd);

    InteractableToAdd->DisableHitbox();
    
    const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
                                          EAttachmentRule::KeepWorld, false);
    ItemActor->AttachToComponent(this, Rules); 

    
    OnRep_Inventory();
	OnAddedToInventory.Broadcast(InteractableToAdd);
    
    return true;
}

bool UContainerComponent::TryAddContainerContentsToInventory(UContainerComponent* SourceContainerComp)
{
	if (!GetOwner()->HasAuthority()) return false;
	if (!Data.IsValid() || !IsValid(SourceContainerComp)) return false;
	if (IsFull() || !bAllowAdd || !SourceContainerComp->GetAllowRemove()) return false;

    RemoveNullsFromInventory();
	SourceContainerComp->RemoveNullsFromInventory();
	
	if (SourceContainerComp->IsEmpty()) return false;
	if (GetInventorySize() + SourceContainerComp->GetInventorySize() > Data->GetMaxCapacity()) return false;
    
	const TArray<TScriptInterface<IItemInterface>>& InInventory = SourceContainerComp->GetInventory();
	// All contents of the source must be compatible, no partial transfer
	for (const TScriptInterface InventoryItem : InInventory)
	{
		if (!InventoryItem && !InventoryItem->HasMatchingInteractableTag(Data->GetAcceptedTags()))
		{
			return false;
		}
	}
    
	// Iterate backwards because removing
	for (int32 i = InInventory.Num() - 1; i >= 0; --i)
	{
		TryAddToInventory(InInventory[i], SourceContainerComp);
	}
    
	return true;
}

bool UContainerComponent::TryRemoveFromInventory(const TScriptInterface<IItemInterface> InteractableToRemove)
{
	if (!GetOwner()->HasAuthority()) return false;
	if (!bAllowRemove) return false;
	
	if (GetAttachParentActor())
	{
		GetAttachParentActor()->ForceNetUpdate();	
	}
	
    if (Inventory.Remove(InteractableToRemove) <= 0) return false;
	
	// InteractableToRemove->EnableHitbox();
	
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