// 

#include "FrogGameMode.h"
#include "UObject/ConstructorHelpers.h"

// void AFrogGameMode::SpawnNiagaraSystem(
// 	const UObject* WorldContextObject,
// 	UNiagaraSystem* NiagaraSystem,
// 	const FVector Location,
// 	const FRotator Rotation,
// 	const FVector Scale,
// 	USceneComponent* AttachToComponent,
// 	const FName AttachPointName,
// 	const bool bAutoDestroy,
// 	const bool bAbsoluteTransform
// )
// {
// 	if (!WorldContextObject || !NiagaraSystem) return;
//
// 	ServerSpawnNiagaraSystem(WorldContextObject, NiagaraSystem, Location, Rotation, Scale, AttachToComponent,
// 	                         AttachPointName, bAutoDestroy, bAbsoluteTransform);
//
// 	// Client prediction
// 	if (IsLocalPlayerController())
// 	{
// 		SpawnNiagaraSystemInternal(WorldContextObject, NiagaraSystem, Location, Rotation, Scale, AttachToComponent,
// 		                           AttachPointName, bAutoDestroy, bAbsoluteTransform);
// 	}
// }
//
// void AFrogGameMode::ServerSpawnNiagaraSystem_Implementation(
// 	const UObject* WorldContextObject,
// 	UNiagaraSystem* NiagaraSystem,
// 	const FVector Location,
// 	const FRotator Rotation,
// 	const FVector Scale,
// 	USceneComponent* AttachToComponent,
// 	const FName AttachPointName,
// 	const bool bAutoDestroy,
// 	const bool bAbsoluteTransform
// )
// {
// 	MulticastSpawnNiagaraSystem(WorldContextObject, NiagaraSystem, Location, Rotation, Scale, AttachToComponent,
// 	                            AttachPointName, bAutoDestroy, bAbsoluteTransform);
// }
//
// void AFrogGameMode::MulticastSpawnNiagaraSystem_Implementation(
// 	const UObject* WorldContextObject,
// 	UNiagaraSystem* NiagaraSystem,
// 	const FVector Location,
// 	const FRotator Rotation,
// 	const FVector Scale,
// 	USceneComponent* AttachToComponent,
// 	const FName AttachPointName,
// 	const bool bAutoDestroy,
// 	const bool bAbsoluteTransform
// )
// {
// 	if (!IsLocalPlayerController())
// 	{
// 		SpawnNiagaraSystemInternal(WorldContextObject, NiagaraSystem, Location, Rotation, Scale, AttachToComponent,
// 								   AttachPointName, bAutoDestroy, bAbsoluteTransform);
// 	}
// 	FString RoleName = UEnum::GetValueAsString(GetLocalRole());
// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Local Role: %s"), *RoleName));
// }
//
// void AFrogGameMode::SpawnNiagaraSystemInternal(
// 	const UObject* WorldContextObject,
// 	UNiagaraSystem* NiagaraSystem,
// 	const FVector& Location,
// 	const FRotator& Rotation,
// 	const FVector& Scale,
// 	USceneComponent* AttachToComponent,
// 	const FName AttachPointName,
// 	const bool bAutoDestroy,
// 	const bool bAbsoluteTransform
// )
// {
// 	if (!NiagaraSystem || !WorldContextObject) return;
//
// 	const UWorld* World = WorldContextObject->GetWorld();
// 	if (!World) return;
//
// 	if (AttachToComponent) // Spawn Attached
// 	{
// 		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
// 			NiagaraSystem,
// 			AttachToComponent,
// 			AttachPointName,
// 			Location,
// 			Rotation,
// 			bAbsoluteTransform ? EAttachLocation::KeepWorldPosition : EAttachLocation::KeepRelativeOffset,
// 			bAutoDestroy
// 		);
// 		if (NiagaraComp && !bAbsoluteTransform)
// 		{
// 			NiagaraComp->SetRelativeScale3D(Scale);
// 		}
// 		else if (NiagaraComp)
// 		{
// 			NiagaraComp->SetWorldScale3D(Scale);
// 		}
// 	}
// 	else // Spawn at Location
// 	{
// 		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 			World,
// 			NiagaraSystem,
// 			Location,
// 			Rotation,
// 			Scale,
// 			bAutoDestroy,
// 			true // auto-activate
// 		);
// 	}
// }