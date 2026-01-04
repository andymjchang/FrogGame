// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidgetComponent.h"


// Sets default values for this component's properties
UInteractableWidgetComponent::UInteractableWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
}


