#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Rooms/RoomDefinition.h"
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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeCard(UTexture2D* InTexture, FText InTitle, URoomDefinition* InRoomDefinition);

protected:
	// --- Bound Widgets --- //
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CardImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CardText;
	
	UPROPERTY(EditAnywhere, Category = "User Settings", meta = (ClampMin = "0", ClampMax = "1"))
	FLinearColor HoverTintColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f); 
	
	UPROPERTY(EditAnywhere, Category = "User Settings", meta = (ClampMin = "0", ClampMax = "1"))
	FLinearColor NormalTintColor = FLinearColor(1.f, 1.f, 1.f, 1.0f); 
	
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
};