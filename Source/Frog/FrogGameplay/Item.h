// Fill out your copyright notice in the Description page of Project Settings.
// POSSBILY DEPRECATED
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Item.generated.h"

class UBoxComponent;
class UInteractableComponent;
class UPrimitiveComponent;

UCLASS()
class FROG_API AItem : public AInteractable
{
	GENERATED_BODY()
    
public:
	AItem();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

protected:

};
