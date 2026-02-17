// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "OrderCounter.generated.h"

class AFrogGameState;

UCLASS()
class FROG_API AOrderCounter : public AContainer
{
	GENERATED_BODY()

public:
	AOrderCounter();

protected:
	virtual void BeginPlay() override;
	virtual void HandleAddedToInventory(AInteractable* Interactable) override;
	
protected:
	TWeakObjectPtr<AFrogGameState> GameState;
};
