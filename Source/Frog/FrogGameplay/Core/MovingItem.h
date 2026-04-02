// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "ItemInterface.h"
#include "GameFramework/Character.h"
#include "MovingItem.generated.h"

struct FGameplayTag;
class UCapsuleComponent;
class UBoxComponent;
class AItem;
class UItemData;

UCLASS()
class FROG_API AMovingItem : public ACharacter, public IInteractableInterface, public IItemInterface
{
	GENERATED_BODY()

public:
	AMovingItem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual FVector GetInteractableLocation() const override;
	virtual void StartInteract() override;

	virtual void EnableHitbox() override;
	virtual void DisableHitbox() override;
	
	virtual UItemData* GetData() const override { return Data; }
	virtual TScriptInterface<IItemInterface> GetOfferedInteractable() const override { return OfferedInteractable; }
	virtual UMeshComponent* GetInteractableMesh() const override { return GetMesh(); }
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void EventAddedToAnotherInventory() override;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_bIsHitboxEnabled();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;

	UPROPERTY(Transient)
	TScriptInterface<IItemInterface> OfferedInteractable;
	
	UPROPERTY(ReplicatedUsing = OnRep_bIsHitboxEnabled)
	bool bIsHitboxEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> InteractHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CollisionComponent;
};
