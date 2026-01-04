// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryElementWidget.h"
#include "Components/HorizontalBox.h"

void UInventoryWidget::UpdateInventoryWidget(const TArray<AInteractable*>& Inventory)
{
	const int32 InventorySize = Inventory.Num();
	if (GEngine)
	{
		const FString DebugMessage = FString::Printf(TEXT("size %d"), InventorySize);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage);
		const FString DebugMessage2 = FString::Printf(TEXT("elements %d"), InventoryElements.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage2);
	}
	// Update Visuals
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

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
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