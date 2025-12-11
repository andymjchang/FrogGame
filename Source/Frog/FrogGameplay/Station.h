// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Station.generated.h"

class AItem;
class AFrogCharacter;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AStation : public AInteractable
{
	GENERATED_BODY()
    
public:
	AStation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract(AFrogCharacter* Interactor);
	virtual bool TryAddItem(AItem* Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Variables")
	int MaxCapacity = 4;
	int CurrentCapacity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Variables")
	TArray<AItem*> Items;
};