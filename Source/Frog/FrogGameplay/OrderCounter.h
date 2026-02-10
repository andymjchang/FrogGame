// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "OrderCounter.generated.h"

class AFrogGameState;

UCLASS()
class FROG_API AOrderCounter : public AItem
{
	GENERATED_BODY()

public:
	AOrderCounter();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleInteractableAdded(AItem* Interactable);
	
protected:
	TWeakObjectPtr<AFrogGameState> GameState;
};
