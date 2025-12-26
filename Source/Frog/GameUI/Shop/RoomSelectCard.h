#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "RoomSelectCard.generated.h"

// Forward declarations to avoid heavy includes in the header
class UCommonTextBlock;
class UImage;
class UTexture2D;

/**
 * A CommonUI Button that displays a dynamic Image and Text.
 */
UCLASS()
class FROG_API URoomSelectCard : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	/** * Sets the image and text for this card. 
	 * Call this after creating the widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeCard(UTexture2D* InTexture, FText InTitle);

protected:
	// --- Bound Widgets --- //
	// The names here must match the names in the Widget Blueprint exactly.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CardImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CardText;
};