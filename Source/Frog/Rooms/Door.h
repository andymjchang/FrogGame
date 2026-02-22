// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Item.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

enum class ERoomDirection : uint8;
class UInteractableWidgetComponent;
class UProgressTrackingComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnProgressComplete, ERoomDirection, FacingDirection);

class UBoxComponent;

UCLASS()
class FROG_API ADoor : public AItem
{
	GENERATED_BODY()

public:
	ADoor();

	void SetActive(bool bIsTrue);

	FORCEINLINE void SetFacingDirection(const ERoomDirection InDirection) { FacingDirection = InDirection; }
	
	// Delegates
	FOnProgressComplete OnProgressComplete;
	
protected:
	virtual void BeginPlay() override;

	virtual void StartInteract() override;
	virtual void StopInteract() override;
	
	void ServerAttemptPurchase();
	
	UFUNCTION()
	void HandleProgressComplete();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProgressTrackingComponent> ProgressTracker;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractableWidgetComponent> ProgressWidgetComponent;
	
	int32 BuyPrice = 0;
	
	ERoomDirection FacingDirection;
};
