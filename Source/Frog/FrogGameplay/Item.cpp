#include "Item.h"
#include "Components/PrimitiveComponent.h"
#include "FrogGameplay/InteractableComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
    
	// Create the InteractableComponent
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
    RootComponent = InteractableComponent;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Bind to the delegates
	if (InteractableComponent)
	{
		InteractableComponent->StartInteract.AddDynamic(this, &AItem::OnStartInteract);
		InteractableComponent->StopInteract.AddDynamic(this, &AItem::OnStopInteract);
	}
}

void AItem::OnStartInteract(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Started interacting with: %s"), *OtherActor->GetName()));
}

void AItem::OnStopInteract(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stopped interacting with: %s"), *OtherActor->GetName()));
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ITEM"))); 
}

UInteractableComponent* AItem::GetInteractableComponent()
{
	return InteractableComponent;
}