// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Container.h"
#include "FrogGameplay/Core/ItemInterface.h"
#include "Spawner.generated.h"

UCLASS()
class FROG_API ASpawner : public AContainer
{
	GENERATED_BODY()

public:
	ASpawner();

protected:
	virtual void BeginPlay() override;
	virtual void HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings", meta = (MustImplement = "/Script/Frog.ItemInterface"))
	TSubclassOf<AActor> InteractableClassToSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	float ReplenishDelay = 3.f;
	
protected:
	FTimerHandle ReplenishTimerHandle;

private:
	void SpawnAndReplenish();
};