// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogGameMode.h"

#include "FrogGameState.h"
#include "GameFramework/PlayerState.h"

void AFrogGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	AFrogGameState* FrogGameState = GetGameState<AFrogGameState>();
	if (FrogGameState && NewPlayer && NewPlayer->PlayerState)
	{
		const FUniqueNetIdRepl UniqueId = NewPlayer->PlayerState->GetUniqueId();
		if (UniqueId.IsValid()) 
		{
			FrogGameState->AddNewPlayer(UniqueId);
		}
	}
}
