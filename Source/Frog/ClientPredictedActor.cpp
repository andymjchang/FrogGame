// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPredictedActor.h"

#include "FrogController.h"
#include "Net/UnrealNetwork.h"

AClientPredictedActor::AClientPredictedActor(): bIsPredictedCopy(false), ClientActorID(0), FollowedServerActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AClientPredictedActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ActorID
	DOREPLIFETIME(AClientPredictedActor, ClientActorID);
}

void AClientPredictedActor::BeginPlay()
{
	// This could be:
	// 1. An instance on the listen server: nothing to do 
	// 2. A predictive instance on the owning client : look up the predictive record by ID, update predicted
	// 3. A replicated instance on the owning client : look up the predictive record by ID, update replicated
	// 4. A replicated instance on a non-owning client : nothing to do

	if (IsLocallyOwned() && GetWorld()->GetNetMode() == NM_Client)
	{
		if (const auto Controller = Cast<AFrogController>(GEngine->GetFirstLocalPlayerController(GetWorld())))
		{
			// If either of these paths matches us up with our counterpart, will call LinkReplicatedWithPredicted
			if (bIsPredictedCopy)
			{
				// Register ourselves so we can match up
				Controller->SetPredictedActor(ClientActorID, this);
			}
			else
			{
				// We're the server copy, having been replicated back to the owning client
				Controller->SetPredictedActorReplicatedActor(ClientActorID, this);
			}
		}
	}

	Super::BeginPlay();

}

void AClientPredictedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(FollowedServerActor))
	{
		UpdateFromFollowedActor(FollowedServerActor, DeltaTime);
	}
}

uint32 AClientPredictedActor::GenerateClientID(const UObject* WorldContext)
{
	if (const auto Controller = Cast<AFrogController>(GEngine->GetFirstLocalPlayerController(WorldContext->GetWorld())))
	{
		return Controller->RequestPredictedActorID();
	}
	check(false)
	return 0;
}

bool AClientPredictedActor::IsLocallyOwned() const
{
	if (const auto World = GetWorld())
	{
		if (const auto Controller = GEngine->GetFirstLocalPlayerController(World))
		{
			return IsOwnedBy(Controller);
		}
	}
	return false;
}

void AClientPredictedActor::LinkReplicatedWithPredicted(
	AClientPredictedActor* PredictedActor)
{
	// We're the server copy, having been replicated back to the owning client
	// The client predicted version will be our visual
	// Hide ourselves
	//SetActorHiddenInGame(true);
	PredictedActor->SetActorScale3D(FVector(1.5, 1.5, 1.5));
	// Also set the client version to not collide or react in any way, server will do this
	PredictedActor->SetActorEnableCollision(false);
	//PredictedActor->FollowReplicatedActor(this);
	FollowReplicatedActor(PredictedActor);
}

void AClientPredictedActor::UpdateFromFollowedActor(AClientPredictedActor* FollowedActor, float DeltaTime)
{
	// Keep the visual actor pulled towards the replicated one
	SetActorLocation(
		FMath::VInterpTo(GetActorLocation(), FollowedActor->GetActorLocation(), DeltaTime, 10));

	SetActorRotation(
		FMath::RInterpTo(GetActorRotation(), FollowedActor->GetActorRotation(), DeltaTime, 10));
}

void AClientPredictedActor::FollowReplicatedActor(AClientPredictedActor* ReplicatedActor)
{
	// Store the actor we're following
	FollowedServerActor = ReplicatedActor;
    
	// Listen for destruction
	if (ReplicatedActor)
	{
		ReplicatedActor->OnDestroyed.AddDynamic(this, &AClientPredictedActor::OnReplicatedActorDestroyed);
	}
}

void AClientPredictedActor::OnReplicatedActorDestroyed(AActor* DestroyedActor)
{
	// Clear our reference to the destroyed actor
	FollowedServerActor = nullptr;
    
	// Since the replicated (server) actor is gone, we should clean up the predicted (client) actor too
	// This prevents orphaned predicted actors from remaining in the world
	Destroy();
}

void AClientPredictedActor::BeginDestroy()
{
	// Clean up delegate binding if the replicated actor still exists
	if (FollowedServerActor && IsValid(FollowedServerActor))
	{
		FollowedServerActor->OnDestroyed.RemoveDynamic(this, &AClientPredictedActor::OnReplicatedActorDestroyed);
	}
    
	Super::BeginDestroy();
}

void AClientPredictedActor::SetIdentifier(const uint32 ID)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SETID %u"), ID));
	ClientActorID = ID;
}

void AClientPredictedActor::SetIsPredictedCopy(const bool Value)
{
	bIsPredictedCopy = Value;
}

