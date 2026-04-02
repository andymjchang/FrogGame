#include "FrogAIController.h"
#include "Components/StateTreeAIComponent.h"

AFrogAIController::AFrogAIController()
{
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTree"));
}