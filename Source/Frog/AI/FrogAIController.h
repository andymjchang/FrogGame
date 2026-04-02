#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "FrogAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class FROG_API AFrogAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFrogAIController();
	
	UStateTreeAIComponent* GetStateTree() const { return StateTreeAIComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};