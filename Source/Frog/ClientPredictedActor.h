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
	bool bIsClientCopy;
	
	UPROPERTY(Replicated)
	uint32 ClientActorID;

	UPROPERTY()
	AClientPredictedActor* FollowedClientActor;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
	
	static uint32 GenerateClientID(const UObject* WorldContext);
	
	bool IsLocallyOwned() const;
	void LinkServerToClientActor(AClientPredictedActor* ClientActor);
	void LerpToFollowedClientActor(const AClientPredictedActor* ClientActor, float DeltaTime);
	void SetClientID(uint32 ID);
	void SetIsClientCopy(bool Value);

	UFUNCTION()
	void OnReplicatedActorDestroyed(AActor* DestroyedActor);
	
};
