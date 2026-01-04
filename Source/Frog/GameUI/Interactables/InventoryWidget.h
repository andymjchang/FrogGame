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
	void UpdateInventoryWidget(const TArray<AInteractable*>& Inventory);
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TopRowHBox;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BottomRowHBox;
	
	UPROPERTY()
	TArray<UInventoryElementWidget*> InventoryElements;
};
