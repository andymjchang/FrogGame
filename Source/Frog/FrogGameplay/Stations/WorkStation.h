// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/Station.h"
#include "WorkStation.generated.h"

class UBoxComponent;

UCLASS()
class FROG_API AWorkStation : public AStation
{
	GENERATED_BODY()

public:
	AWorkStation();

	virtual void StartWork(APlayerState* PlayerState);
	virtual void StopWork(APlayerState* PlayerState);

};