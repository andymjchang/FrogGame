// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogTongue.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UFrogTongue::UFrogTongue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true); 

	// ... 
}

void UFrogTongue::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts
void UFrogTongue::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFrogTongue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

