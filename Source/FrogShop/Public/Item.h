// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Item.generated.h"

class UPrimitiveComponent;

UCLASS()
class FROGSHOP_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
    
public:
	AItem();

protected:
	virtual void BeginPlay() override;
    
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void On_Interact() override;

protected:
	// Reference to collision component (set in Blueprint or found at runtime)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<UPrimitiveComponent> InteractionCollision;
};