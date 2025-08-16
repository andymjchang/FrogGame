// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FrogGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UFrogGameInstance : public UGameInstance
{
	GENERATED_BODY()
	void CreateServer();
};
