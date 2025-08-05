// // Fill out your copyright notice in the Description page of Project Settings.
//
// #include "ClientPredictedActor.h"
//
// #include "FrogCharacter/FrogController.h"
// #include "Net/UnrealNetwork.h"
//
// AClientPredictedActor::AClientPredictedActor(): bIsClientCopy(false), ClientActorID(0), FollowedClientActor(nullptr)
// {
// 	PrimaryActorTick.bCanEverTick = true;
// }
//
// void AClientPredictedActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
// 	DOREPLIFETIME(AClientPredictedActor, ClientActorID);
// }
//
// /// If client copy: Add this actor to the controller's struct
// /// If server copy: Look up the client actor and follow it 
// void AClientPredictedActor::BeginPlay()
// {
// 	if (IsLocallyOwned() && GetWorld()->GetNetMode() == NM_Client)
// 	{
// 		if (const auto Controller = Cast<AFrogController>(GEngine->GetFirstLocalPlayerController(GetWorld())))
// 		{
// 			if (bIsClientCopy)
// 			{
// 				Controller->RegisterClientActor(ClientActorID, this);
// 			}
// 			else
// 			{
// 				Controller->FindClientActorToFollow(ClientActorID, this);
// 			}
// 		}
// 	}
//
// 	Super::BeginPlay();
//
// }
//
// void AClientPredictedActor::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	if (IsValid(FollowedClientActor))
// 	{
// 		LerpToFollowedClientActor(FollowedClientActor, DeltaTime);
// 	}
// }
//
// uint32 AClientPredictedActor::GenerateClientID(const UObject* WorldContext)
// {
// 	if (const auto Controller = Cast<AFrogController>(GEngine->GetFirstLocalPlayerController(WorldContext->GetWorld())))
// 	{
// 		return Controller->GetClientActorID();
// 	}
// 	return 0;
// }
//
// bool AClientPredictedActor::IsLocallyOwned() const
// {
// 	if (const auto World = GetWorld())
// 	{
// 		if (const auto Controller = GEngine->GetFirstLocalPlayerController(World))
// 		{
// 			return IsOwnedBy(Controller);
// 		}
// 	}
// 	return false;
// }
//
// void AClientPredictedActor::LinkServerToClientActor(
// 	AClientPredictedActor* ClientActor)
// {
// 	SetActorHiddenInGame(true);
// 	ClientActor->SetActorEnableCollision(false);
// 	FollowedClientActor = ClientActor;
//
// 	if (ClientActor)
// 	{
// 		ClientActor->OnDestroyed.AddDynamic(this, &AClientPredictedActor::OnReplicatedActorDestroyed);
// 	}
// }
//
// void AClientPredictedActor::LerpToFollowedClientActor(const AClientPredictedActor* ClientActor, const float DeltaTime)
// {
// 	SetActorLocation(
// 		FMath::VInterpTo(GetActorLocation(), ClientActor->GetActorLocation(), DeltaTime, 10));
//
// 	SetActorRotation(
// 		FMath::RInterpTo(GetActorRotation(), ClientActor->GetActorRotation(), DeltaTime, 10));
// }
//
// void AClientPredictedActor::OnReplicatedActorDestroyed(AActor* DestroyedActor)
// {
// 	FollowedClientActor = nullptr;
// 	Destroy();
// }
//
// void AClientPredictedActor::BeginDestroy()
// {
// 	if (FollowedClientActor && IsValid(FollowedClientActor))
// 	{
// 		FollowedClientActor->OnDestroyed.RemoveDynamic(this, &AClientPredictedActor::OnReplicatedActorDestroyed);
// 	}
//     
// 	Super::BeginDestroy();
// }
//
// void AClientPredictedActor::SetClientID(const uint32 ID)
// {
// 	ClientActorID = ID;
// }
//
// void AClientPredictedActor::SetIsClientCopy(const bool Value)
// {
// 	bIsClientCopy = Value;
// }
//
