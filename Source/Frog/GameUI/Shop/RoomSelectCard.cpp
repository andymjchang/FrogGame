#include "RoomSelectCard.h"
#include "Components/Image.h"
#include "CommonTextBlock.h"
#include "Rooms/RoomDefinition.h"

void URoomSelectCard::InitializeCard(UTexture2D* InTexture, FText InTitle, URoomDefinition* InRoomDefinition)
{
	// Update the Text
	if (CardText)
	{
		CardText->SetText(InTitle);
	}

	// Update the Image
	if (CardImage)
	{
		if (InTexture)
		{
			CardImage->SetBrushFromTexture(InTexture);
			CardImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CardImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URoomSelectCard::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (CardImage)
	{
		CardImage->SetColorAndOpacity(HoverTintColor);
	}
}

void URoomSelectCard::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (CardImage)
	{
		CardImage->SetColorAndOpacity(NormalTintColor);
	}
}