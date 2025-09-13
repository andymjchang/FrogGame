// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogCharacter/FrogInventoryComponent.h"

UFrogInventoryComponent::UFrogInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UFrogInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UFrogInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

