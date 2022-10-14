
#include "GMPlayerActionWidget.h"
#include "Components/TextBlock.h"
#include "GridMovement/Combat/GMCombatManager.h"

void UGMPlayerActionWidget::SetButtonName(const FString& Name)
{
	GetNameTextBlock()->SetText(FText::FromString(Name));
}

void UGMPlayerActionWidget::ClickButton()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("Clicked button"), true,
									 FVector2D(1.f));

	//CombatManager->SetUnitState(ButtonIndex);
}

