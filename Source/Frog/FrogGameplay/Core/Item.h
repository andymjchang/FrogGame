// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UItemData;
class UBoxComponent;

UCLASS()
class FROG_API AItem : public AActor, public IInteractableInterface, public IItemInterface
{
	GENERATED_BODY()

public:
	AItem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual FVector GetInteractableLocation() const override;
	virtual void EnableHitbox() override;
	virtual void DisableHitbox() override;
	
	void SetItemDormancy(bool bDormant);

protected:
	virtual void BeginPlay() override;

	TScriptInterface<IItemInterface> SpawnItem(TSubclassOf<AActor> ClassToSpawn, const FTransform& SpawnTransform);
	
	UFUNCTION()
	void OnRep_bIsHitboxEnabled();
	
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

public:
	virtual UItemData* GetData() const override { return Data; }
	virtual TScriptInterface<IItemInterface> GetOfferedInteractable() const override { return OfferedInteractable; }
	virtual UMeshComponent* GetInteractableMesh() const override { return InteractableMesh; }
};
