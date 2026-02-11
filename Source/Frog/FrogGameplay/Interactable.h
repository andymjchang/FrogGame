// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	virtual void Interact();

	void StartHighlight();
	void StopHighlight();
	
	void EnableInteractable();
	void DisableInteractable();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UMaterialInterface> OverlayMaterial;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

public:
	FORCEINLINE UItemData* GetData() const { return Data; }
};
