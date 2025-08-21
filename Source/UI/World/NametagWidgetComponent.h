// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NametagWidgetComponent.generated.h"


class UNametagWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UI_API UNametagWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UNametagWidgetComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNametagWidget* NametagWidget;
};
