// Fill out your copyright notice in the Description page of Project Settings.


#include "BillboardWidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBillboardWidgetComponent::UBillboardWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Space = EWidgetSpace::World;
	bDrawAtDesiredSize = true;
	SetCastShadow(false);
}

void UBillboardWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		FVector CameraLocation = CameraManager->GetTransformComponent()->GetComponentLocation();
		FVector WidgetLocation = GetComponentLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
	
		SetWorldRotation(LookAtRotation);
	}

}

