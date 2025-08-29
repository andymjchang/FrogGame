// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTestAbility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Unit/ProjectileSpawnerComponent.h"
#include "Unit/UnitInterface.h"

void UEnemyTestAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
										const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!AvatarPawn)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// --- Accessing the Blackboard ---
	AAIController* AIController = Cast<AAIController>(AvatarPawn->GetController());
	UBlackboardComponent* BlackboardComp = AIController ? AIController->GetBlackboardComponent() : nullptr;

	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find Blackboard Component."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* TargetActor = nullptr;
	UObject* BlackboardObject = BlackboardComp->GetValueAsObject(TEXT("TargetActor"));
	if (BlackboardObject)
	{
		TargetActor = Cast<AActor>(BlackboardObject);
	} 
	IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
	if (!Unit || !TargetActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector TargetDirection = TargetLocation - AvatarPawn->GetActorLocation();
    
	if (UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent())
	{
		// Use the location from the blackboard instead of the actor's location
		ProjectileSpawner->RequestSpawnProjectile(ProjectileClass,
		   AvatarPawn->GetActorLocation(), FRotator::ZeroRotator, TargetDirection);
	}
    
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}