#include "InventoryItemData.h"
#include "AbilitySystemComponent.h"

UInventoryItemData::UInventoryItemData() : ItemIcon(nullptr), AbilityInputKey()
{
	ItemName = FText::FromString("Default Item");
	ItemDescription = FText::FromString("Default Description");
}

void UInventoryItemData::OnItemAdded(AActor* Owner)
{
	if (!Owner) return;
	
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
			if (AbilityInputKey != EAbilityInputID::None)
			{
				FGameplayAbilitySpec* ExistingAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromInputID(static_cast<int32>(AbilityInputKey));
				if (ExistingAbilitySpec)
				{
					AbilitySystemComponent->ClearAbility(ExistingAbilitySpec->Handle);
				}
			}
			AbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GrantedAbility, 1, static_cast<int32>(AbilityInputKey), this));
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
