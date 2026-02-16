// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InventoryElementWidget.generated.h"

class UImage;
class UInteractableData;
/**
 * 
 */
UCLASS()
class FROG_API UInventoryElementWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void SetElement(const UInteractableData* Data);
	void DisableElement();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
};
