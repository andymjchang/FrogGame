// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Item.h"
#include "GameFramework/Actor.h"
#include "MovingItemSpawner.generated.h"

class AMovingItem;

UCLASS()
class FROG_API AMovingItemSpawner : public AItem
{
	GENERATED_BODY()

public:
	AMovingItemSpawner();

protected:
	void SpawnMovingItem();
	
	UFUNCTION()
	void StartSpawnTimer();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TSubclassOf<AMovingItem> InteractableClassToSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	float ReplenishDelay = 3.f;
	
protected:
	FTimerHandle ReplenishTimerHandle;
};
