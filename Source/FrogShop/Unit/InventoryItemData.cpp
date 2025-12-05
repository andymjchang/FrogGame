#include "InventoryItemData.h"
#include "AbilitySystemComponent.h"
#include "FrogCharacter/FrogCharacter.h"
#include "UI/HUD/FrogHUD.h"

UInventoryItemData::UInventoryItemData() : ItemIcon(nullptr), AbilityInputKey()
{
	ItemName = FText::FromString("Default Item");
	ItemDescription = FText::FromString("Default Description");
}

void UInventoryItemData::OnItemAdded(AActor* Owner)
{
	if (!Owner) return;
	
	if (UAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UAbilitySystemComponent>())
	{
		if (PassiveGameplayEffect)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(PassiveGameplayEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				PassiveEffectHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

		if (GrantedAbility)
		{
			if (AbilityInputKey != EAbilityInputID::None)
			{
				if (const FGameplayAbilitySpec* OldAbilitySpec = AbilitySystem->FindAbilitySpecFromInputID(static_cast<int32>(AbilityInputKey)))
				{
					AbilitySystem->ClearAbility(OldAbilitySpec->Handle);
				}
			}
			AbilitySpecHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(GrantedAbility, 1, static_cast<int32>(AbilityInputKey), this));

			const UFrogHUD* FrogHUD = Cast<AFrogCharacter>(Owner)->GetFrogHUD();
			if (FrogHUD && AbilitySpecHandle.IsValid())
			{
				FrogHUD->AssignAbilityToUISlot(AbilitySpecHandle, AbilitySystem);
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
