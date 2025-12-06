// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NametagWidgetComponent.generated.h"

struct FOnAttributeChangeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UNametagWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
