// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UBoxComponent;

UCLASS()
class FROG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();
	
	FORCEINLINE bool IsMoveable() const { return Moveable; }

	// Interaction
	void EnableInteractable();
	void DisableInteractable();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Item Settings")
	int Capacity;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item Settings")
	bool Moveable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
