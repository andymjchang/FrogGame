// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogGameplay/Core/InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

enum class ERoomDirection : uint8;
class UInteractableWidgetComponent;
class UProgressTrackingComponent;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnProgressComplete, ERoomDirection, FacingDirection, int32, UnlockPrice);

class UBoxComponent;

UCLASS()
class FROG_API ADoor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ADoor();

	void SetHitboxActiveState(bool bIsHitboxActive);

	FORCEINLINE void SetFacingDirection(const ERoomDirection InDirection) { FacingDirection = InDirection; }
	
	// Delegates
	FOnProgressComplete OnProgressComplete;
	
protected:
	virtual void BeginPlay() override;

	virtual void StartInteract() override;
	virtual void StopInteract() override;
	virtual void StartHighlight(UMaterialInterface* HighlightMaterial) override;
	virtual void StopHighlight() override;
	virtual FVector GetInteractableLocation() override;
	
	UFUNCTION()
	void HandleProgressComplete();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractHitbox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProgressTrackingComponent> ProgressTracker;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInteractableWidgetComponent> ProgressWidgetComponent;
	
	int32 UnlockPrice = 0;
	
	ERoomDirection FacingDirection;
};
