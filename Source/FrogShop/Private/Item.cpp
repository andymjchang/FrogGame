#include "Item.h"
#include "Components/PrimitiveComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	// Or if you know the specific name in Blueprint, use
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionCollision = Cast<UPrimitiveComponent>(GetDefaultSubobjectByName(TEXT("ItemCollisionComponent")));
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		On_Interact();
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::On_Interact()
{
	// Your interaction logic here
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ITEM"))); 
}