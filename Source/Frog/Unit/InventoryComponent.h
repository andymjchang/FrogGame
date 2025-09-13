#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemData;
class UInventoryItem;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UInventoryItemData* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UInventoryItemData* Item);
	
	const TArray<TObjectPtr<UInventoryItemData>>& GetItems() const { return Items; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UInventoryItemData>> Items;
};
