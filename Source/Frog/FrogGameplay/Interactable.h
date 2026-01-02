// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

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
	int MaxCapacity = 4;

	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool Moveable;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	FGameplayTagContainer CompatibleInteractableTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	FGameplayTagContainer OwnedInteractableTags;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractable> OfferedInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInteractable*> Inventory;
	

	
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsMoveable() const { return Moveable; }
	FORCEINLINE int GetInventorySize() const { return Inventory.Num(); }
	FORCEINLINE FGameplayTagContainer& GetOwnedItemTags() { return OwnedInteractableTags; }
	FORCEINLINE AInteractable* GetOfferedInteractable() { return OfferedInteractable.Get(); };
};
