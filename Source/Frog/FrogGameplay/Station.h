// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Container.h"
#include "GameplayTagContainer.h"
#include "Station.generated.h"

class UProgressTrackingComponent;
class UStationProgressBar;
class UItemData;
class AFrogCharacter;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AStation : public AContainer
{
	GENERATED_BODY()
    
public:
	AStation();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleProcessingComplete();
	virtual void HandleRemovedFromInventory(AItem* Interactable) override;

	FGameplayTagContainer GatherAllTags() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProgressTrackingComponent> ProgressTracker;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> ProgressWidgetComponent;
	
};