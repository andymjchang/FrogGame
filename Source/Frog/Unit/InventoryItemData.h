#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "InventoryItemData.generated.h"

/**
 * Represents a single item in the inventory.
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UInventoryItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	UInventoryItemData();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UGameplayEffect> PassiveGameplayEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UGameplayAbility> GrantedAbility;
	
	virtual void OnItemAdded(AActor* Owner);
	virtual void OnItemRemoved(AActor* Owner);

protected:
	FActiveGameplayEffectHandle PassiveEffectHandle;
	FGameplayAbilitySpecHandle AbilitySpecHandle;
};
