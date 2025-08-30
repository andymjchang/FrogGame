// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_FindClosestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FrogCharacter/FrogCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

UBTT_FindClosestPlayer::UBTT_FindClosestPlayer()
{
    NodeName = "Find Closest Player";
}

EBTNodeResult::Type UBTT_FindClosestPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    const APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) return EBTNodeResult::Failed;

    // Find closest player class
    AActor* ClosestPlayer = nullptr;
    float MinDistanceSquared = FLT_MAX; 
    TArray<AActor*> FoundPlayers;
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFrogCharacter::StaticClass(), FoundPlayers);

    if (FoundPlayers.Num() == 0) return EBTNodeResult::Failed;

    const FGameplayTag DownedTag = FGameplayTag::RequestGameplayTag(FName("Frog.State.Downed"));
    
    for (AActor* PlayerActor : FoundPlayers)
    {
       if (!PlayerActor) continue;

       // Check if the player has the "Downed" tag. If so, ignore them.
       IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(PlayerActor);
       if (AbilitySystemInterface)
       {
          UAbilitySystemComponent* AbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent();
          if (AbilitySystemComp && AbilitySystemComp->HasMatchingGameplayTag(DownedTag))
          {
             continue; // Skip this downed player
          }
       }
       
       const float DistanceSquared = FVector::DistSquared(ControlledPawn->GetActorLocation(), PlayerActor->GetActorLocation());

       if (DistanceSquared < MinDistanceSquared)
       {
          MinDistanceSquared = DistanceSquared;
          ClosestPlayer = PlayerActor;
       }
    }

    // Write to blackboard if we found a valid (not downed) player.
    if (ClosestPlayer)
    {
       if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
       {
          BlackboardComp->SetValueAsObject(TargetPlayerKey.SelectedKeyName, ClosestPlayer);
          return EBTNodeResult::Succeeded;
       }
    }
    
    // If ClosestPlayer is null (no players found, or all players are downed), fail the task.
    return EBTNodeResult::Failed;
}

FString UBTT_FindClosestPlayer::GetStaticDescription() const
{
    return FString::Printf(TEXT("Target Player: %s"), *TargetPlayerKey.SelectedKeyName.ToString());
}
