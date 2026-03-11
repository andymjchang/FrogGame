// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrogController.generated.h"

class UUserBaseWidget;
class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class FROG_API AFrogController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UUserBaseWidget* GetFrogBaseWidget() const { return FrogBaseWidget; }

protected:
	virtual void BeginPlay() override;
	
protected:
	// HUD
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UUserBaseWidget> BaseWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserBaseWidget> FrogBaseWidget;
	
	
};
