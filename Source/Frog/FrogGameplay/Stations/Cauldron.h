// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Station.h"
#include "Cauldron.generated.h"

class AFrogCharacter;

UCLASS()
class FROG_API ACauldron : public AStation
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACauldron();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
