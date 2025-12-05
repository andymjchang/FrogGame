#include "InventoryComponent.h"
#include "InventoryItemData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(UInventoryItemData* Item)
{
	if (Item)
	{
		Items.Add(Item);
		Item->OnItemAdded(GetOwner());
	}
}

void UInventoryComponent::RemoveItem(UInventoryItemData* Item)
{

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
