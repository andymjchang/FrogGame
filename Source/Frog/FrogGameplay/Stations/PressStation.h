// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Stations/WorkStation.h"
#include "PressStation.generated.h"

UCLASS()
class FROG_API APressStation : public AWorkStation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APressStation();
	virtual void StartWork() override;

protected:
	//pct added to the progress per press
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	float PctAddedPerPress = 10.0f;
	// Called when the game starts or when spawned

public:
	// Called every frame
};
