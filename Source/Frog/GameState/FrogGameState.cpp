#include "FrogGameState.h"

#include "Net/UnrealNetwork.h"

AFrogGameState::AFrogGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Initialize Defaults
    Money = 0;
    PhaseEndTime = 0.0f;
    CurrentPhase = EFrogGamePhase::Night;
}

void AFrogGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFrogGameState, Money);
    DOREPLIFETIME(AFrogGameState, CurrentPhase);
    DOREPLIFETIME(AFrogGameState, PhaseEndTime);
}

void AFrogGameState::ServerTryUnlockDoor(ERoomDirection FacingDirection)
{
    if (!HasAuthority()) return;
	
    AFrogGameState* GameState = GetWorld()->GetGameState<AFrogGameState>();
    if (!IsValid(GameState)) return;
	
   // TODO:  
}

TSubclassOf<AItem> AFrogGameState::GetRecipeResultClass(
    const FGameplayTagContainer& InteractableTags) const
{
    if (IngredientMap)
    {
        return IngredientMap->LookupInteractableClassByTagContainer(InteractableTags).ItemClass;
    }
    return nullptr;
}

UItemData* AFrogGameState::GetRecipeResultData(const FGameplayTagContainer& InteractableTags) const
{
    if (IngredientMap)
    {
        return IngredientMap->LookupInteractableClassByTagContainer(InteractableTags).ItemData;
    }
    return nullptr;
}

void AFrogGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (!HasAuthority()) return;
    
    if (CurrentPhase == EFrogGamePhase::Day)
    {
        if (GetServerWorldTimeSeconds() >= PhaseEndTime)
        {
            TriggerNightPhase();
        }
    }
}

void AFrogGameState::TriggerDayPhase(float DayDuration)
{
    if (!HasAuthority()) return;

    CurrentPhase = EFrogGamePhase::Day;
    PhaseEndTime = GetServerWorldTimeSeconds() + DayDuration;
    
    OnRep_CurrentPhase();
    OnRep_PhaseEndTime();
}

void AFrogGameState::TriggerNightPhase()
{
    if (!HasAuthority()) return;

    CurrentPhase = EFrogGamePhase::Night;
    PhaseEndTime = 0.0f;
    
    OnRep_CurrentPhase();
    OnRep_PhaseEndTime();
}

void AFrogGameState::ServerAddMoney(const int32 Amount)
{
    if (!HasAuthority()) return;
    if (Amount <= 0) return;
    
    Money += Amount;
    OnRep_Money();
}

void AFrogGameState::ServerTrySubtractMoney(const int32 Amount)
{
    if (!HasAuthority()) return;
    if (Amount > Money) return;
    
    Money -= Amount;
    OnRep_Money();
}

float AFrogGameState::GetTimeRemaining() const
{
    if (CurrentPhase != EFrogGamePhase::Day || PhaseEndTime <= 0.0f) return 0.0f;
    return FMath::Max(PhaseEndTime - GetServerWorldTimeSeconds(), 0.0f);
}

// OnRep Functions
void AFrogGameState::OnRep_Money()
{
    OnMoneyChanged.Broadcast(Money);
}

void AFrogGameState::OnRep_CurrentPhase()
{
    OnPhaseChanged.Broadcast(CurrentPhase);
}

void AFrogGameState::OnRep_PhaseEndTime()
{
    OnPhaseEndTimeUpdated.Broadcast(PhaseEndTime);
}