// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UWidgetComponent;
class UInteractableData;
class UBoxComponent;

UCLASS()
class FROG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	
public:
	void EnableInteractable();
	void DisableInteractable();
	bool TryAddToInventory(AInteractable* InteractableToAdd);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UInteractableData> Data;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractable> OfferedInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInteractable*> Inventory;
	
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InventoryWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootSceneComponent;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UInteractableData* GetData() { return Data; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE AInteractable* GetOfferedInteractable() { return OfferedInteractable.Get(); };
};
