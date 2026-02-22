#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "IngredientMap.h"
#include "FrogGameplay/Item.h"
#include "FrogGameState.generated.h"

class ARoomManager;

UENUM(BlueprintType)
enum class EFrogGamePhase : uint8
{
    Day,
    Night
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EFrogGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseEndTimeUpdated, float, NewTime);

UCLASS()
class FROG_API AFrogGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AFrogGameState();
    
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Delegates
    FOnMoneyChanged OnMoneyChanged;
    FOnPhaseChanged OnPhaseChanged;
    FOnPhaseEndTimeUpdated OnPhaseEndTimeUpdated;
     
    // Server only functions
    void ServerAddMoney(const int32 Amount);
    void ServerTrySubtractMoney(const int32 Amount);
    
    // Client functions
    TSubclassOf<AItem> GetRecipeResultClass(const FGameplayTagContainer& InteractableTags) const;
    UItemData* GetRecipeResultData(const FGameplayTagContainer& InteractableTags) const;
    float GetTimeRemaining() const;
    
    FORCEINLINE int32 GetMoney() const { return Money; }
    FORCEINLINE int32 GetPhaseEndTime() const { return PhaseEndTime; }
    FORCEINLINE EFrogGamePhase GetCurrentPhase() const { return CurrentPhase; }
    // FORCEINLINE ARoomManager* GetRoomManager() { return RoomManager.Get(); }
    // FORCEINLINE void SetRoomManager(ARoomManager* InRoomManager) { RoomManager = InRoomManager; }
    
protected:  
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UIngredientMap> IngredientMap;
    
    // Replicated variables
    UPROPERTY(ReplicatedUsing = OnRep_Money)
    int32 Money = 0;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentPhase)
    EFrogGamePhase CurrentPhase = EFrogGamePhase::Day;

    UPROPERTY(ReplicatedUsing = OnRep_PhaseEndTime)
    float PhaseEndTime;

    // UPROPERTY()
    // TWeakObjectPtr<ARoomManager> RoomManager;
    
protected:
    // Server only functions
    void TriggerDayPhase(float DayDuration);
    void TriggerNightPhase();

    // OnRep functions
    UFUNCTION()
    void OnRep_Money();
    UFUNCTION()
    void OnRep_CurrentPhase();
    UFUNCTION()
    void OnRep_PhaseEndTime();
};