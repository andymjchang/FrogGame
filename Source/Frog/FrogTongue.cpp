// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogTongue.h"

UFrogTongue::UFrogTongue()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(false); 
}

void UFrogTongue::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UFrogTongue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



