// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"

#include "ContainerComponent.h"
#include "ItemData.h"
#include "ProgressTrackingComponent.h"
#include "GameState/FrogGameState.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "GameUI/Interactables/InteractableWidgetComponent.h"

AStation::AStation()
{
    PrimaryActorTick.bCanEverTick = false;
    SetNetUpdateFrequency(10.f);
    SetNetDormancy(DORM_Awake);
    
    // Progress Tracking Component
    ProgressTracker = CreateDefaultSubobject<UProgressTrackingComponent>(TEXT("ProgressTrackingComponent"));
    
    // Progress Bar Widget Component
    ProgressWidgetComponent = CreateDefaultSubobject<UInteractableWidgetComponent>(TEXT("ProgressBarWidgetComponent"));
    ProgressWidgetComponent->SetupAttachment(RootComponent);
    ProgressWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -600.0f));
    ProgressWidgetComponent->SetDrawSize(FIntPoint(100, 20));
    
    // Interactable Mesh 
    InteractableMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AStation::BeginPlay()
{
    Super::BeginPlay();
    
    if (IsValid(ProgressWidgetComponent) && IsValid(ProgressTracker))
    {
        ProgressTracker->OnCompletion.BindDynamic(this, &AStation::HandleProcessingComplete);
        ProgressTracker->SetProgressWidgetReference(ProgressWidgetComponent->GetWidget());
    }
}

FGameplayTagContainer AStation::GatherAllTags() const
{
    FGameplayTagContainer AllTags;
    
    if (IsValid(Data))
    {
        AllTags.AppendTags(Data->GetOwnedTags());
    }
    
    for (const TScriptInterface Item : ContainerComponent->GetInventory())
    {
        if (Item && IsValid(Item->GetData()))
        {
            AllTags.AppendTags(Item->GetData()->GetOwnedTags());
        }
    }
    
    return AllTags;
}

void AStation::HandleProcessingComplete()
{
    if (!HasAuthority()) return;
    
    const AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState)) return;
    
    ContainerComponent->SetAllowRemove(true);
    ContainerComponent->ClearInventory();
    
    const FGameplayTagContainer AllTags = GatherAllTags();
    if (const TSubclassOf<AActor> ResultClass = GameState->GetRecipeResultClass(AllTags))
    {
        if (const TScriptInterface<IItemInterface> NewItem = SpawnAndAddToInventory(ResultClass))
        {
            OfferedInteractable = NewItem;
        }
    }
    else
    {
        OfferedInteractable = this;
    }
}

void AStation::HandleRemovedFromInventory(const TScriptInterface<IItemInterface>& Interactable)
{
    Super::HandleRemovedFromInventory(Interactable);

    if (IsValid(ProgressTracker))
    {
        ProgressTracker->ResetProgress();
    }
}
