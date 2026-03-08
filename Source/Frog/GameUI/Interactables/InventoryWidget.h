// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryWidget.generated.h"

class AItem;
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
	virtual void NativeOnInitialized() override;
	
public:
	void UpdateInventoryWidget(const TArray<AItem*>& Inventory);

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TopRowHBox;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BottomRowHBox;
	
	UPROPERTY()
	TArray<UInventoryElementWidget*> InventoryElements;
};
