// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTestAbility.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter/EnemyCharacter.h"
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
	
	AAIController* AIController = Cast<AAIController>(AvatarPawn->GetController());
	UBlackboardComponent* BlackboardComp = AIController ? AIController->GetBlackboardComponent() : nullptr;

	if (!BlackboardComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AActor* TargetActor = nullptr;
	if (UObject* BlackboardObject = BlackboardComp->GetValueAsObject(TEXT("TargetActor")))
	{
		TargetActor = Cast<AActor>(BlackboardObject);
	} 
	IUnitInterface* Unit = Cast<IUnitInterface>(ActorInfo->AvatarActor.Get());
	if (!Unit || !TargetActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector TargetDirection = (TargetLocation - AvatarPawn->GetActorLocation()).GetSafeNormal();
	const FVector TempOffset = FVector(0, 0, 50.f);
    
	if (UProjectileSpawnerComponent* ProjectileSpawner = Unit->GetProjectileSpawnerComponent())
	{
		if (UAbilitySystemComponent* OwningAbilitySystem = Cast<IAbilitySystemInterface>(ActorInfo->AvatarActor.Get())->GetAbilitySystemComponent())
		{
			const FVector SpawnLocation = AvatarPawn->GetActorLocation() + TempOffset;
			const FRotator SpawnRotation = FRotator(0, 0, 0);
			
			ProjectileSpawner->RequestSpawnProjectile(ProjectileClass, SpawnLocation, SpawnRotation, TargetDirection,
			                                          nullptr, OwningAbilitySystem);
		}
	}
    
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
} 