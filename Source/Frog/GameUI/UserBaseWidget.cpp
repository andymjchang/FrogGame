#include "UserBaseWidget.h"

void UUserBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!MenuStack)
	{
		UE_LOG(LogTemp, Error, TEXT("MenuStack is missing in %s! Ensure you added a CommonActivatableWidgetStack and named it 'MenuStack' in the Designer."), *GetName());
	}
}

void UUserBaseWidget::PushWidget(TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!MenuStack) return;

	if (WidgetClass)
	{
		MenuStack->AddWidget(WidgetClass);
	}
}

void UUserBaseWidget::PopWidget()
{
	if (!MenuStack) return;
	
	if (UCommonActivatableWidget* ActiveWidget = MenuStack->GetActiveWidget())
	{
		ActiveWidget->DeactivateWidget();
	}
}