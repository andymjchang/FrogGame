// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UItemData;
class UBoxComponent;

UCLASS()
class FROG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	
public:
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;

	virtual void Interact();

	void StartHighlight();
	void StopHighlight();
	
	void EnableInteractable();
	void DisableInteractable();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UMaterialInterface> OverlayMaterial;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractable> OfferedInteractable;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

public:
	FORCEINLINE UItemData* GetData() const { return Data; }
	FORCEINLINE AInteractable* GetOfferedInteractable() const { return OfferedInteractable.Get(); };
};
