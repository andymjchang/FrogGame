// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "Components/ActorComponent.h"
#include "FrogTongue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTongueEndReached);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROG_API UFrogTongue : public UCableComponent
{
	GENERATED_BODY()

public:	
	UFrogTongue();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	FOnTongueEndReached OnTongueEndReached;
};
