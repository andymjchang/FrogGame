// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractableWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UInteractableWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractableWidgetComponent();

};
