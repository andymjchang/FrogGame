// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Station.generated.h"

class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AStation : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:
	AStation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Interactable Interface
	UFUNCTION()
	virtual void OnStartInteract(AActor* OtherActor) override;
	UFUNCTION()
	virtual void OnStopInteract(AActor* OtherActor) override;
	UFUNCTION()
	virtual UInteractableComponent* GetInteractableComponent() override;
	
protected:
	// Interaction hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Variables")
	int Capacity = 1;
	
	TArray<FString> Inventory;
	
	FTimerHandle InteractionTimerHandle;

	TObjectPtr<UInteractableComponent> InteractableComponent;
};