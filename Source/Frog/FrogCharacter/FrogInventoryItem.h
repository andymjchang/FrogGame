#pragma once

#include "FrogInventoryItem.generated.h"

class UGameplayAbility;
struct FGameplayEffectSpecHandle;

UCLASS(BlueprintType)
class UFrogInventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayEffectSpecHandle> GrantedEffects;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GrantedAbility;
};
