#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveForwardWithTurnLimit.generated.h"

/**
 * Custom Behavior Tree Task to move an AI pawn only in its forward direction
 * with a limited turning speed, similar to a vehicle.
 */
UCLASS()
class FROG_API UBTT_MoveForwardWithTurnLimit : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveForwardWithTurnLimit();

protected:
	/** This function is called when the task is executed by the Behavior Tree. */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** This function is called every frame while the task is active. */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    
	/** This function is called when the task is aborted. */
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) override;

	/** Blackboard key selector for the target actor. */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** How fast the AI can turn towards the target. Lower values mean a larger turn radius. */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float TurnSpeed = 2.0f;

	/** How close the AI needs to be to the target to consider the task complete. */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AcceptanceRadius = 100.0f;
    
private:
	/** Returns a string description of the node for the Behavior Tree editor. */
	virtual FString GetStaticDescription() const override;
};