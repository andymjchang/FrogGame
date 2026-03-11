// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UItemData;
class UBoxComponent;

UCLASS()
class FROG_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AItem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual FVector GetInteractableLocation() const override;
	virtual void StartInteract() override;
	
	bool HasMatchingInteractableTag(const FGameplayTagContainer& AcceptedTags) const;
	void DisableHitbox();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "User Settings")
	TObjectPtr<UItemData> Data;

	UPROPERTY(Transient)
	TObjectPtr<AItem> OfferedInteractable;
	
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
	FORCEINLINE UItemData* GetData() const { return Data; }
	FORCEINLINE AItem* GetOfferedInteractable() const { return OfferedInteractable.Get(); }
	FORCEINLINE virtual UStaticMeshComponent* GetInteractableMesh() const override { return InteractableMesh.Get(); }
};
