// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Station.generated.h"

class UPrimitiveComponent;

UCLASS()
class FROGSHOP_API AStation : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:
	AStation();

protected:
	virtual void BeginPlay() override;
    
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void On_Interact() override;
	void CompleteInteraction();

protected:
	// Reference to collision component (set in Blueprint or found at runtime)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<UPrimitiveComponent> InteractionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Variables")
	int Capacity = 1;
	TArray<FString> Inventory;
	FTimerHandle InteractionTimerHandle;
};