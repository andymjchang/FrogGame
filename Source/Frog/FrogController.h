// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrogController.generated.h"

class AClientPredictedActor;

struct FPredictedActorInfo
{
	uint32 ClientActorID = 0;
	TWeakObjectPtr<AClientPredictedActor> ClientActor;
	TWeakObjectPtr<AClientPredictedActor> ServerActor;
};
/**
 * 
 */
UCLASS()
class FROG_API AFrogController : public APlayerController
{
	GENERATED_BODY()

protected:
	static constexpr int32 Max_Predicted_Actors = 32;
	uint32 NextPredictedActorID = 1;
    
	/// Client actors are stored in a hash map for O(1) lookup + a circular buffer for memory management
	TMap<uint32, int32> IDToSlotMap; 
	FPredictedActorInfo ClientActors[Max_Predicted_Actors];
	int32 NextSlotIndex = 0;
	
public:
	uint32 GetClientActorID();
	FPredictedActorInfo* FindActorInfo(uint32 ID);
	void RegisterClientActor(uint32 ID, AClientPredictedActor* ClientActor);
	void FindClientActorToFollow(uint32 ID, AClientPredictedActor* ServerActor);
};
