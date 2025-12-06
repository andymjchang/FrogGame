// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAttributeSet.h"
#include "FrogAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FROGSHOP_API UFrogAttributeSet : public UUnitAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Mana
	UPROPERTY(BlueprintReadOnly, Category="Unit Attributes", ReplicatedUsing=OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UFrogAttributeSet, Mana);

	// Max Mana
	UPROPERTY(BlueprintReadOnly, Category="Unit Attributes", ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UFrogAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
