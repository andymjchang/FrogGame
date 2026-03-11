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
	APressStation();
	virtual void StartWork(APlayerState* PlayerState) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	float PctAddedPerPress = 10.0f;
};
