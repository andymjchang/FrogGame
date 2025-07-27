// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrogController.generated.h"

class AClientPredictedActor;

struct FPredictedActorInfo
{
	/// The identifier. This is separate from the actor because we can create the ID first,
	/// then delay creating the client actor, so we need to know the *intended* ID
	uint32 ClientActorID = 0;

	/// The client predicted actor. Hopefully should be created before the server one replicates
	/// back to us, but in the case of a mis-prediction of lag, the server might send us the actor first
	TWeakObjectPtr<AClientPredictedActor> PredictedActor;

	/// The server replicated actor. 
	TWeakObjectPtr<AClientPredictedActor> ReplicatedActor;
};
/**
 * 
 */
UCLASS()
class FROG_API AFrogController : public APlayerController
{
	GENERATED_BODY()

protected:
	uint32 NextPredictedActorID = 0;
	
	/// Client predicted actors that are owned locally, waiting for the server copy to match up with
	TArray<FPredictedActorInfo> PredictedActors;
public:
	uint32 RequestPredictedActorID();
	void SetPredictedActor(uint32 ID, AClientPredictedActor* PredictedActor);
	void SetPredictedActorReplicatedActor(uint32 ID, AClientPredictedActor* ReplicatedActor);
};
