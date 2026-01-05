// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "GameplayTagContainer.h"
#include "Station.generated.h"

class UInteractableData;
class AItem;
class AFrogCharacter;
class UBoxComponent;
class UPrimitiveComponent;
class UInteractableWidgetComponent;

UCLASS()
class FROG_API AStation : public AInteractable
{
	GENERATED_BODY()
    
public:
	AStation();

	virtual bool TryAddToInventory(AInteractable* InteractableToAdd) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteractableWidgetComponent> ProgressBarWidgetComponent;
	
protected:
	virtual void BeginPlay() override;

	// Timer handle for processing
	FTimerHandle ProcessingTimerHandle;

	// Processing duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Station")
	float ProcessingDuration = 3.0f;

	// Function called when timer completes
	UFUNCTION()
	void OnProcessingComplete();

	// Helper to gather all tags from station + inventory
	FGameplayTagContainer GatherAllTags() const;
};