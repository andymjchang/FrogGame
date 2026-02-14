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
	
	UFUNCTION()
	void HandleInteractableAdded(AContainer* Interactable);
	
protected:
	TWeakObjectPtr<AFrogGameState> GameState;
};
