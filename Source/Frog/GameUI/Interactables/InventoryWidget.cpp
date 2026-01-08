// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryElementWidget.h"
#include "Components/HorizontalBox.h"

void UInventoryWidget::UpdateInventoryWidget(const TArray<AInteractable*>& Inventory)
{ 
	for (int32 i = 0; i < InventoryElements.Num(); i++)
	{
		
		if (!IsValid(InventoryElements[i])) continue;

		if (Inventory.IsValidIndex(i) && IsValid(Inventory[i]))
		{
			InventoryElements[i]->SetElement(Inventory[i]->GetData());
			
		}
		else
		{
			InventoryElements[i]->DisableElement();
		}
	}
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
    
	if (!IsValid(TopRowHBox) or !IsValid(BottomRowHBox)) return;
	InventoryElements.Empty();

	// Add children from a box
	auto AddSlotsFromBox = [&](const UHorizontalBox* Box)
	{
		if (!IsValid(Box)) return;
		for (UWidget* Child : Box->GetAllChildren())
		{
			if (UInventoryElementWidget* Slot = Cast<UInventoryElementWidget>(Child))
			{
				InventoryElements.Add(Slot);
			}
		}
	};

	AddSlotsFromBox(TopRowHBox);
	AddSlotsFromBox(BottomRowHBox);
}