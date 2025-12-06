// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FindClosestPlayer.generated.h"

/**
 * Behavior Tree Task to find the closest player pawn to the AI and store it in a Blackboard key.
 * This task is server-authoritative and will fail if executed on a client.
 */
UCLASS()
class FROG_API UBTT_FindClosestPlayer : public UBTTaskNode 
{
	GENERATED_BODY()

public:
	UBTT_FindClosestPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetPlayerKey;

private:
	virtual FString GetStaticDescription() const override;
};