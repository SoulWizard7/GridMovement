#include "GMGameMode.h"

#include "UI/GMOverlayWidget.h"

AGMGameMode::AGMGameMode()
{
	DefaultPawnClass = nullptr;
}

AGMGameMode* AGMGameMode::Get(UObject* WorldContext)
{
	return WorldContext->GetWorld()->GetAuthGameMode<AGMGameMode>();
}

void AGMGameMode::StartMatch()
{
	OverlayWidget = NewObject<UGMOverlayWidget>(this, OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	
	Super::StartMatch();
}