// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Container.h"
#include "Spawner.generated.h"

UCLASS()
class FROG_API ASpawner : public AContainer
{
	GENERATED_BODY()

public:
	ASpawner();

protected:
	virtual void BeginPlay() override;
	virtual void HandleRemovedFromInventory(AItem* Interactable) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<AItem> InteractableClassToSpawn;

private:
	void SpawnAndReplenish();
};