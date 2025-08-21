// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NametagWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UI_API UNametagWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	void SetHealthBarPercent(const float Percent) const;
};
