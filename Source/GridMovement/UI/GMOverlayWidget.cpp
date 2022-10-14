
#include "GMOverlayWidget.h"

#include "GMEmptyWidget.h"
#include "GMEndTurnWidget.h"
#include "GMPlayerActionWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UGMOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ActionWidgets.SetNum(4);
}

void UGMOverlayWidget::AddActionButton(int ButtonIndex)
{
	static FName ButtonNames[] = {
		TEXT("Button0Slot"),
		TEXT("Button1Slot"),
		TEXT("Button2Slot"),
		TEXT("Button3Slot"),
	};	
	
	UGMPlayerActionWidget* ActionWidget = NewObject<UGMPlayerActionWidget>(this, PlayerActionWidgetClass);
	ActionWidget->ButtonIndex = ButtonIndex;
	ActionWidgets[ButtonIndex] = ActionWidget;
	
	SetContentForSlot(ButtonNames[ButtonIndex], ActionWidget);
}

void UGMOverlayWidget::RemoveActionButton(int ButtonIndex)
{
	static FName ButtonNames[] = {
		TEXT("Button0Slot"),
		TEXT("Button1Slot"),
		TEXT("Button2Slot"),
		TEXT("Button3Slot"),
	};
	ActionWidgets[ButtonIndex] = nullptr;
	
	UGMEmptyWidget* EmptyWidget = NewObject<UGMEmptyWidget>();
	SetContentForSlot(ButtonNames[ButtonIndex], EmptyWidget);
}

void UGMOverlayWidget::AddEndTurnButton()
{
	static FName EndTurnName = TEXT("EndTurnSlot");
	
	UGMEndTurnWidget* Widget = NewObject<UGMEndTurnWidget>(this, EndTurnWidgetClass);
	EndTurnWidget = Widget;

	SetContentForSlot(EndTurnName, Widget);	
}

void UGMOverlayWidget::RemoveEndTurnButton()
{
	static FName EndTurnName = TEXT("EndTurnSlot");
	EndTurnWidget = nullptr;
	UGMEmptyWidget* EmptyWidget = NewObject<UGMEmptyWidget>();
	
	SetContentForSlot(EndTurnName, EmptyWidget);
}
