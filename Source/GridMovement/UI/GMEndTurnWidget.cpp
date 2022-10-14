#include "GMEndTurnWidget.h"

#include "GridMovement/Combat/GMCombatManager.h"

void UGMEndTurnWidget::ClickButton()
{
	if(CombatManager)
	{		
		CombatManager->EndPlayerTurn();
	}
}
