// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrogGameMode.h"
#include "FrogCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFrogGameMode::AFrogGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
