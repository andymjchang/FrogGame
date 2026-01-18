// Fill out your copyright notice in the Description page of Project Settings.
// THIS CLASS IS NOT USED

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BillboardWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROG_API UBillboardWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UBillboardWidgetComponent();

protected:

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
