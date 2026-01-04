// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InventoryWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UInventoryWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UInventoryWidgetComponent();
};
