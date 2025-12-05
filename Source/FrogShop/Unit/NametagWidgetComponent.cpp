// Fill out your copyright notice in the Description page of Project Settings.


#include "NametagWidgetComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/UnitAttributeSet.h"
#include "UI/World/NametagWidget.h"

void UNametagWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    
    AActor* Owner = GetOwner();
    if (!Owner) return;
    
    const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner);
    if (!AbilitySystemInterface) return;

    if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UUnitAttributeSet::GetHealthAttribute())
           .AddUObject(this, &UNametagWidgetComponent::OnHealthChanged);
    }

    if (const UNametagWidget* NametagWidget = Cast<UNametagWidget>(GetWidget()))
    {
        NametagWidget->SetHealthBarPercent(1.0);
    }
}

void UNametagWidgetComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;
    
    const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner);
    if (!AbilitySystemInterface) return;
    
    if (const UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
    {
        if (const UNametagWidget* NametagWidget = Cast<UNametagWidget>(GetWidget()))
        {
            const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UUnitAttributeSet::GetHealthAttribute());
            const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UUnitAttributeSet::GetMaxHealthAttribute());

            NametagWidget->SetHealthBarPercent(CurrentHealth / MaxHealth);
        }
    }
}

void UNametagWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (AActor* Owner = GetOwner())
    {
        if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner))
        {
            if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
            {
                AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UUnitAttributeSet::GetHealthAttribute()).RemoveAll(this);
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}