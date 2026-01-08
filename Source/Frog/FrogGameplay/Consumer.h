// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Consumer.generated.h"

UCLASS()
class FROG_API AConsumer : public AInteractable
{
	GENERATED_BODY()

public:
	AConsumer();

protected:
	virtual void BeginPlay() override;
	
	void HandleInteractableAdded(AInteractable* Interactable);

};
