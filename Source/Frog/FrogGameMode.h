// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FrogGameMode.generated.h"

UCLASS(minimalapi)
class AFrogGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
// 	/**
// 	 * Spawns a Niagara system either attached or at a world location.
// 	 *
// 	 * @param WorldContextObject   Context object (e.g., this). Required for World access.
// 	 * @param NiagaraSystem        The Niagara system asset to spawn.
// 	 * @param Location             World location, or relative if attached and bAbsoluteTransform is false
// 	 * @param Rotation             [OPTIONAL] World rotation, or relative if attached and bAbsoluteTransform is false
// 	 * @param Scale                [OPTIONAL] scale for the effect.
// 	 * @param AttachToComponent    [OPTIONAL] component to attach to.
// 	 * @param AttachPointName      [OPTIONAL] socket/attach point name.
// 	 * @param bAutoDestroy         Whether to auto-destroy the component when finished.
// 	 * @param bAbsoluteTransform   [OPTIONAL] Whether location/rotation are world-absolute when attaching.
// 	 * @return                     The spawned Niagara component (or nullptr if failed).
// 	 */
// 	UFUNCTION(BlueprintCallable, Category = "VFX", meta = (WorldContext = "WorldContextObject"))
// 	void SpawnNiagaraSystem(
// 		const UObject* WorldContextObject,
// 		UNiagaraSystem* NiagaraSystem,
// 		FVector Location = FVector::ZeroVector,
// 		FRotator Rotation = FRotator::ZeroRotator,
// 		FVector Scale = FVector(1.0f, 1.0f, 1.0f),
// 		USceneComponent* AttachToComponent = nullptr,
// 		FName AttachPointName = NAME_None,
// 		bool bAutoDestroy = true,
// 		bool bAbsoluteTransform = false
// 	);
//
// protected:
// 	UFUNCTION(Server, Reliable)
// 	void ServerSpawnNiagaraSystem(const UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
// 	                                            FVector Location, FRotator Rotation, FVector Scale,
// 	                                            USceneComponent* AttachToComponent, FName AttachPointName,
// 	                                            bool bAutoDestroy, bool bAbsoluteTransform);
//
// 	UFUNCTION(NetMulticast, Reliable)
// 	void MulticastSpawnNiagaraSystem(const UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
// 	                                               FVector Location, FRotator Rotation, FVector Scale,
// 	                                               USceneComponent* AttachToComponent, FName AttachPointName,
// 	                                               bool bAutoDestroy, bool bAbsoluteTransform);
//
// 	void SpawnNiagaraSystemInternal(const UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
// 	                                                     const FVector& Location, const FRotator& Rotation, const FVector& Scale,
// 	                                                     USceneComponent* AttachToComponent, FName AttachPointName, bool bAutoDestroy,
// 	                                                     bool bAbsoluteTransform);
};



