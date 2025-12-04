// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypes.h"
#include "RoomActor.generated.h"


UCLASS()
class FROGSHOP_API ARoomActor : public AActor
{
	GENERATED_BODY()

public:
	ARoomActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWallArray(const TArray<bool>& InputArray);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDoorArray(const TArray<EDoorTypes>& InputArray);
};
