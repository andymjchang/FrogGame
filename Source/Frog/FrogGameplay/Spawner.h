// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container.h"
#include "Spawner.generated.h"

UCLASS()
class FROG_API ASpawner : public AContainer
{
	GENERATED_BODY()

public:
	ASpawner();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleInteractableRemoved(AInteractable* RemovedInteractable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<AInteractable> InteractableClassToSpawn;

private:
	void SpawnAndReplenish();
};