// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Station.h"
#include "AutoStation.generated.h"

UCLASS()
class FROG_API AAutoStation : public AStation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAutoStation();

protected:
	virtual void HandleAddedToInventory(const TScriptInterface<IItemInterface>& InteractableToAdd) override;

};
