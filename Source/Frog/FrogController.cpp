// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogController.h"

#include "ClientPredictedActor.h"

uint32 AFrogController::RequestPredictedActorID()
{
	const uint32 NewID = NextPredictedActorID++;
	PredictedActors.Add(FPredictedActorInfo( { NewID }) );
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("New Predicted ID: %u"), NewID));
	}
	return NewID;
}

void AFrogController::SetPredictedActor(uint32 ID, AClientPredictedActor* PredictedActor)
{
	if (const auto PredictedActorInfo = PredictedActors.FindByPredicate([ID](const FPredictedActorInfo& Info)
		{
			return Info.ClientActorID == ID;
		}))
	{
		ensure(!PredictedActorInfo->PredictedActor.IsValid());
		PredictedActorInfo->PredictedActor = PredictedActor;

		// If both are valid, link up
		if (PredictedActorInfo->PredictedActor.IsValid() && PredictedActorInfo->ReplicatedActor.IsValid())
		{
			PredictedActorInfo->ReplicatedActor->LinkReplicatedWithPredicted(PredictedActorInfo->PredictedActor.Get());
		}
	}
}

void AFrogController::SetPredictedActorReplicatedActor(uint32 ID, AClientPredictedActor* ReplicatedActor)
{
	if (auto pInfo = PredictedActors.FindByPredicate([ID](const FPredictedActorInfo& Info)
	{
		return Info.ClientActorID == ID;
	}))
	{
		ensure(!pInfo->ReplicatedActor.IsValid());
		pInfo->ReplicatedActor = ReplicatedActor;

		// If both are valid, link up
		if (pInfo->PredictedActor.IsValid() && pInfo->ReplicatedActor.IsValid())
		{
			pInfo->ReplicatedActor->LinkReplicatedWithPredicted(pInfo->PredictedActor.Get());
		}
	}
}