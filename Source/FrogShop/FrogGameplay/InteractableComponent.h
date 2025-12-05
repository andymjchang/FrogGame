// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

// Declare delegate signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartInteractDelegate, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopInteractDelegate, AActor*, OtherActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FROGSHOP_API UInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent();

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FStartInteractDelegate StartInteract;
    
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FStopInteractDelegate StopInteract;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
				   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
				   bool bFromSweep, const FHitResult& SweepResult);
    
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
				  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
};