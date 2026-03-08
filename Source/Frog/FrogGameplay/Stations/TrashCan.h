// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Container.h"
#include "TrashCan.generated.h"

UCLASS()
class FROG_API ATrashCan : public AContainer
{
	GENERATED_BODY()

public:
	ATrashCan();

protected:
	virtual void BeginPlay() override;
	virtual void HandleAddedToInventory(AItem* Interactable) override;
};
