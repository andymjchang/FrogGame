// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Spawner.generated.h"

UCLASS()
class FROG_API ASpawner : public AItem
{
	GENERATED_BODY()

public:
	ASpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleInteractableRemoved(AItem* AddedInteractable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<AItem> InteractableClassToSpawn;

private:
	void SpawnAndReplenish();
};