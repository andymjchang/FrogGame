// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Container.h"
#include "Counter.generated.h"

UCLASS()
class FROG_API ACounter : public AContainer
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACounter();
	
protected:
	virtual void HandleAddedToInventory(AItem* Interactable) override;
};
