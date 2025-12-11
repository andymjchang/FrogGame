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

