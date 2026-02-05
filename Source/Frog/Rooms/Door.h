// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UProgressTrackingComponent;
DECLARE_MULTICAST_DELEGATE(FOnProgressComplete);

class UBoxComponent;

UCLASS()
class FROG_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();
	
	void SetActive(bool bIsTrue);
	
	// Delegates
	FOnProgressComplete OnProgressComplete;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> Hitbox;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProgressTrackingComponent> ProgressTracker;
};
