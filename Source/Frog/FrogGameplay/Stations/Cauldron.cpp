// Fill out your copyright notice in the Description page of Project Settings.


#include "Cauldron.h"

#include "FrogCharacter/FrogCharacter.h"


// Sets default values
ACauldron::ACauldron()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACauldron::OnInteract(AFrogCharacter* Interactor)
{
	if (GEngine)
	{
		const FString DebugMessage = FString::Printf(TEXT("Cauldron V2"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
	}
}

// Called when the game starts or when spawned
void ACauldron::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACauldron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

