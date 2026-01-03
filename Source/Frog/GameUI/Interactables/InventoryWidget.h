// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "FrogGameplay/Interactable.h"
#include "InventoryWidget.generated.h"

class UInventoryElementWidget;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class FROG_API UInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateInventory(const TArray<AInteractable>& Inventory);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TopRowHBox;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BottomRowHBox;
	
	UPROPERTY(meta = (BindWidget))
	UInventoryElementWidget* Slot1;
	UPROPERTY(meta = (BindWidget))
	UInventoryElementWidget* Slot2;
	UPROPERTY(meta = (BindWidget))
	UInventoryElementWidget* Slot3;
	UPROPERTY(meta = (BindWidget))
	UInventoryElementWidget* Slot4;
};
