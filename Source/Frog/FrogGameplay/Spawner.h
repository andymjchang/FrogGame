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
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleInteractableRemoved(AContainer* AddedInteractable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<AContainer> InteractableClassToSpawn;

private:
	void SpawnAndReplenish();
};