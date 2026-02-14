// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "Consumer.generated.h"

UCLASS()
class FROG_API AConsumer : public AContainer
{
	GENERATED_BODY()

public:
	AConsumer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleInteractableAdded(AContainer* Interactable);

};
