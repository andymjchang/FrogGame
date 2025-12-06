// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Item.generated.h"

class UBoxComponent;
class UInteractableComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:
	AItem();

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
	virtual UInteractableComponent*  GetInteractableComponent() override;
	
	TObjectPtr<UInteractableComponent> InteractableComponent;
	// virtual void On_Interact() override;

protected:
	// Interaction hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractHitBox;
};
