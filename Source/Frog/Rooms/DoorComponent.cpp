// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorComponent.h"

#include "Components/BoxComponent.h"


// Sets default values for this component's properties
UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(this);
	Hitbox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	// Hitbox->SetCollisionProfileName(TEXT("BlockAll"));
}

