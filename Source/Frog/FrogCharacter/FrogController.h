// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrogController.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UFrogHUD;

/**
 * 
 */
UCLASS()
class FROG_API AFrogController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFrogHUD* GetFrogHUD() const { return FrogHUDWidget; }

	virtual void BeginPlay() override;

protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UFrogHUD> FrogHUDClass;
	
	UPROPERTY()
	TObjectPtr<UFrogHUD> FrogHUDWidget;
};
