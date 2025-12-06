// Fill out your copyright notice in the Description page of Project Settings.


#include "NametagWidgetComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "UI/World/NametagWidget.h"

void UNametagWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    
    AActor* Owner = GetOwner();
    if (!Owner) return;
    
    const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner);
    if (!AbilitySystemInterface) return;

    if (const UNametagWidget* NametagWidget = Cast<UNametagWidget>(GetWidget()))
    {
        NametagWidget->SetHealthBarPercent(1.0);
    }
}

void UNametagWidgetComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{

}

void UNametagWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}