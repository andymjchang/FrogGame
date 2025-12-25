#include "RoomSelectCard.h"
#include "Components/Image.h"
#include "CommonTextBlock.h"

void URoomSelectCard::InitializeCard(UTexture2D* InTexture, FText InTitle)
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
			// Optional: Hide image if no texture is provided
			// CardImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}