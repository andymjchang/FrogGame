// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClientPredictedActor.generated.h"

UCLASS()
class FROG_API AClientPredictedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AClientPredictedActor();

protected:
	virtual void BeginPlay() override;
	bool bIsPredictedCopy;
	UPROPERTY(Replicated)
	uint32 ClientActorID;

	UPROPERTY()
	AClientPredictedActor* FollowedServerActor;

public:	
	virtual void Tick(float DeltaTime) override;
	static uint32 GenerateClientID(const UObject* WorldContext);
	bool IsLocallyOwned() const;
	void LinkReplicatedWithPredicted(AClientPredictedActor* PredictedActor);
	void UpdateFromFollowedActor(AClientPredictedActor* FollowedActor, float DeltaTime);
	
	void FollowReplicatedActor(AClientPredictedActor* ReplicatedActor);

	UFUNCTION()
	void OnReplicatedActorDestroyed(AActor* DestroyedActor);
	
	virtual void BeginDestroy() override;
	void SetIdentifier(uint32 ID);
	void SetIsPredictedCopy(bool Value);
};
