#include "GMEmptyWidget.h"

void UGMEmptyWidget::ClickButton()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("Clicked button"), true,
									 FVector2D(1.f));
}
