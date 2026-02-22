// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryElementWidget.generated.h"

class UImage;
class UItemData;
/**
 * 
 */
UCLASS()
class FROG_API UInventoryElementWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void SetElement(const UItemData* Data);
	void DisableElement();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
};
