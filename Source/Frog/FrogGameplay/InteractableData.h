// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InteractableData.generated.h"

/**
 * 
 */
UCLASS()
class FROG_API UInteractableData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetIsMoveable() const { return IsMoveable; }
	FORCEINLINE FGameplayTagContainer& GetCompatibleTags() { return CompatibleTags; }
	FORCEINLINE FGameplayTagContainer& GetOwnedTags() { return OwnedTags; }
	FORCEINLINE int GetMaxCapacity() const { return MaxCapacity; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	int MaxCapacity = 4;

	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	bool IsMoveable;
	 
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	FGameplayTagContainer CompatibleTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	FGameplayTagContainer OwnedTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Settings")
	TSoftObjectPtr<UTexture2D> Icon;
};
