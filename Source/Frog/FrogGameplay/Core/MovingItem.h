// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "ItemInterface.h"
#include "GameFramework/Pawn.h"
#include "MovingItem.generated.h"

class UBoxComponent;
class AItem;
class UItemData;

UCLASS()
class FROG_API AMovingItem : public APawn, public IInteractableInterface, public IItemInterface
{
	GENERATED_BODY()

public:
	AMovingItem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual FVector GetInteractableLocation() const override;
	virtual void StartInteract() override;

	virtual void EnableHitbox() override;
	virtual void DisableHitbox() override;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;

	UPROPERTY(Transient)
	TScriptInterface<IItemInterface> OfferedInteractable;
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsHitboxEnabled)
	bool bIsHitboxEnabled = true;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractHitBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> InteractableMesh;
	
protected:
	UFUNCTION()
	void OnRep_bIsHitboxEnabled();
	
public:
	virtual UItemData* GetData() const override { return Data; }
	virtual TScriptInterface<IItemInterface> GetOfferedInteractable() const override { return OfferedInteractable; }
	virtual UStaticMeshComponent* GetInteractableMesh() const override { return InteractableMesh.Get(); }
};
