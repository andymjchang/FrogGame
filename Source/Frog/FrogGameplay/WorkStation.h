// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Station.h"
#include "WorkStation.generated.h"

class UBoxComponent;

UCLASS()
class FROG_API AWorkStation : public AStation
{
	GENERATED_BODY()

public:
	AWorkStation();

protected:
	virtual void BeginPlay() override;
	
	virtual void HandleInteractableAdded(AInteractable* InteractableToAdd) override;

public:
	UFUNCTION()
	void OnWorkHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
	UFUNCTION()
	void OnWorkHitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> WorkHitBox;

	float ProcessingDuration = 3.0f;
    
	bool bIsBeingWorked = false;
};