// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Station.generated.h"

class AItem;
class AFrogCharacter;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AStation : public AInteractable
{
	GENERATED_BODY()
    
public:
	AStation();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> ProgressBarWidgetComponent;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract(AFrogCharacter* Interactor);

protected:
};