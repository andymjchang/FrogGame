// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "OrderCounter.generated.h"

class AFrogGameState;

UCLASS()
class FROG_API AOrderCounter : public AInteractable
{
	GENERATED_BODY()

public:
	AOrderCounter();

protected:
	virtual void BeginPlay() override;
	void HandleInteractableAdded(AInteractable* Interactable);
	
protected:
	TWeakObjectPtr<AFrogGameState> GameState;
};
