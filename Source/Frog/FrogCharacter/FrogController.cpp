// Fill out your copyright notice in the Description page of Project Settings.

#include "FrogController.h"
#include "Blueprint/UserWidget.h"
#include "GameUI/UserBaseWidget.h"

void AFrogController::BeginPlay()
{
	if (BaseWidgetClass)
	{
		FrogBaseWidget = CreateWidget<UUserBaseWidget>(this, BaseWidgetClass);
		if (FrogBaseWidget) FrogBaseWidget->AddToViewport();
	}
}
