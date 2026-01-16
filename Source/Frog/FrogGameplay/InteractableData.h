// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InteractableData.generated.h"

// TODO: Identity which data assets are needed before starting a day and pre load
/**
 * 
 */
UCLASS()
class FROG_API UInteractableData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetIsMoveable() const { return IsMoveable; }
	FORCEINLINE int GetMaxCapacity() const { return MaxCapacity; }
	FORCEINLINE int GetSellPrice() const { return SellPrice; }
	FORCEINLINE FGameplayTagContainer& GetAcceptedTags() { return AcceptedTags; }
	FORCEINLINE FGameplayTagContainer& GetOwnedTags() { return OwnedTags; }
	FORCEINLINE UTexture2D* GetIcon() const { return Icon.Get(); }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	int MaxCapacity = 4;

	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool IsMoveable;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	int SellPrice = -1;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	FGameplayTagContainer AcceptedTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	FGameplayTagContainer OwnedTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TObjectPtr<UTexture2D> Icon;
};
