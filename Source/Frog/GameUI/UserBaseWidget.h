#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "UserBaseWidget.generated.h"

/**
 * A main HUD class that manages a stack of UI elements (menus, popups, etc).
 */
UCLASS()
class FROG_API UUserBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/**
	 * Pushes a new Activatable Widget (e.g., a Pause Menu) onto the stack.
	 * This will automatically capture input focus if the widget settings allow.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Navigation")
	void PushWidget(TSubclassOf<UCommonActivatableWidget> WidgetClass);

	/**
	 * Removes the currently top-most active widget from the stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Navigation")
	void PopWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;
};