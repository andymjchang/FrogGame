#include "BTT_MoveForwardWithTurnLimit.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_MoveForwardWithTurnLimit::UBTT_MoveForwardWithTurnLimit()
{
	NodeName = "Move Forward To Actor";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MoveForwardWithTurnLimit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

	return EBTNodeResult::InProgress;
}

void UBTT_MoveForwardWithTurnLimit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// // Get the target Actor from the Blackboard
	// UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
	// AActor* TargetActor = Cast<AActor>(TargetObject);
	//
	// // If the target actor is not valid (e.g., it was destroyed), fail the task
	// if (!TargetActor)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	// 	return;
	// }
	//
	// // Get the location from the valid target actor
	// const FVector TargetLocation = TargetActor->GetActorLocation();
	// const FVector PawnLocation = ControlledPawn->GetActorLocation();
	//
	// // Check if we have reached the destination
	// if (FVector::Dist(PawnLocation, TargetLocation) <= AcceptanceRadius)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	return;
	// }

	// --- Rotation Logic ---
	// const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
	// const FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	// const FRotator NewRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, TurnSpeed);
	// ControlledPawn->SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));

	// --- Movement Logic ---
	ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), 1.0f);
}

EBTNodeResult::Type UBTT_MoveForwardWithTurnLimit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}

FString UBTT_MoveForwardWithTurnLimit::GetStaticDescription() const
{
	return FString::Printf(TEXT("Target Actor: %s\nTurn Speed: %.1f\nAcceptance Radius: %.1f"),
		*TargetActorKey.SelectedKeyName.ToString(), TurnSpeed, AcceptanceRadius);
}