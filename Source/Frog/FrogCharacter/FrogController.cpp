// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogController.h"
#include "Blueprint/UserWidget.h"
#include "GameUI/FrogHUD.h"

void AFrogController::BeginPlay()
{
	if (FrogHUDClass)
	{
		FrogHUDWidget = CreateWidget<UFrogHUD>(this, FrogHUDClass);
		if (FrogHUDWidget) FrogHUDWidget->AddToViewport();
	}
}
