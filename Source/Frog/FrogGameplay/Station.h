// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "GameplayTagContainer.h"
#include "Station.generated.h"

class UStationProgressBar;
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
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteractableWidgetComponent> ProgressBarWidgetComponent;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Station")
	float ProcessingDuration = 3.0f;
	
	UPROPERTY()
	TSoftObjectPtr<UStationProgressBar> ProgressBarWidget;
	
	virtual void HandleInteractableAdded(AInteractable* InteractableToAdd);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnProcessingComplete();

	FGameplayTagContainer GatherAllTags() const;
	
	bool bIsProcessing = false;
	double ProcessStartTime = 0.0f;
	double ProcessEndTime = 0.0f;
};