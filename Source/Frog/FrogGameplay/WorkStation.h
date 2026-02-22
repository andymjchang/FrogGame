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

	virtual void StartWork();
	void virtual StopWork();
	virtual void StopHighlight() override;

protected:
	bool bIsBeingWorked = false;
};