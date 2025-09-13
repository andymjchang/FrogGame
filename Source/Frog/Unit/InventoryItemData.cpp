#include "InventoryItemData.h"
#include "AbilitySystemComponent.h"

UInventoryItemData::UInventoryItemData() : ItemIcon(nullptr)
{
	ItemName = FText::FromString("Default Item");
	ItemDescription = FText::FromString("Default Description");
}

void UInventoryItemData::OnItemAdded(AActor* Owner)
{
	if (Owner)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (PassiveGameplayEffect)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PassiveGameplayEffect, 1, EffectContext);
				if (SpecHandle.IsValid())
				{
					PassiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}

			if (GrantedAbility)
			{
				AbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GrantedAbility, 1, -1, this));
			}
		}
	}
}

void UInventoryItemData::OnItemRemoved(AActor* Owner)
{
	if (Owner)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (PassiveEffectHandle.IsValid())
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(PassiveEffectHandle);
			}

			if (AbilitySpecHandle.IsValid())
			{
				AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
			}
		}
	}
}
