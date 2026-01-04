// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryElementWidget.h"

#include "Components/Image.h"
#include "FrogGameplay/InteractableData.h"

void UInventoryElementWidget::SetElement(const UInteractableData* Data)
{
	if (!IsValid(Data) || !IsValid(Icon)) return;
	
	Icon->SetBrushFromTexture(Data->GetIcon());
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInventoryElementWidget::DisableElement()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
