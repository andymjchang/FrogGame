// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogController.h"

#include "ClientPredictedActor.h"

uint32 AFrogController::GetClientActorID()
{
	const uint32 ID = NextPredictedActorID++;
	const int32 SlotIndex = NextSlotIndex;
	NextSlotIndex = (NextSlotIndex + 1) % Max_Predicted_Actors;
        
	// Remove old mapping if slot was occupied
	if (ClientActors[SlotIndex].ClientActorID != 0)
	{
		IDToSlotMap.Remove(ClientActors[SlotIndex].ClientActorID);
	}
        
	// Set up new entry
	ClientActors[SlotIndex] = FPredictedActorInfo();
	ClientActors[SlotIndex].ClientActorID = ID;
	IDToSlotMap.Add(ID, SlotIndex);
        
	return ID;
}

FPredictedActorInfo* AFrogController::FindActorInfo(const uint32 ID)
{
	if (const int32* SlotPtr = IDToSlotMap.Find(ID))
	{
		return &ClientActors[*SlotPtr];
	}
	return nullptr;
}

void AFrogController::RegisterClientActor(const uint32 ID, AClientPredictedActor* ClientActor)
{
	if (const auto ClientActorInfo = FindActorInfo(ID))
	{
		ensure(!ClientActorInfo->ClientActor.IsValid());
		ClientActorInfo->ClientActor = ClientActor;
	}
}

void AFrogController::FindClientActorToFollow(const uint32 ID, AClientPredictedActor* ServerActor)
{
	if (const auto ClientActorInfo = FindActorInfo(ID))
	{
		ensure(!ClientActorInfo->ServerActor.IsValid());
		ClientActorInfo->ServerActor = ServerActor;
		
		if (ClientActorInfo->ClientActor.IsValid() && ClientActorInfo->ServerActor.IsValid())
		{
			ClientActorInfo->ServerActor->LinkServerToClientActor(ClientActorInfo->ClientActor.Get());
		}
	}
}