#pragma once

#include "GMEmptyWidget.h"
#include "Blueprint/UserWidget.h"
#include "GMOverlayWidget.generated.h"

class UCanvasPanel;
class UGMPlayerActionWidget;
class UGMEndTurnWidget;

UCLASS()
class UGMOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;

	UFUNCTION()
	void AddActionButton(int ButtonIndex);

	UFUNCTION()
	void RemoveActionButton(int ButtonIndex);
	
	UFUNCTION()
	void AddEndTurnButton();

	UFUNCTION()
	void RemoveEndTurnButton();
	
	UFUNCTION(BlueprintImplementableEvent)
	UCanvasPanel* GetMainCanvas();

	UPROPERTY(EditAnywhere, Category = GameOverlay)
	TSubclassOf<UGMPlayerActionWidget> PlayerActionWidgetClass;

	UPROPERTY()
	TArray<UGMPlayerActionWidget*> ActionWidgets;

	UPROPERTY(EditAnywhere, Category = GameOverlay)
	TSubclassOf<UGMEndTurnWidget> EndTurnWidgetClass;

	UPROPERTY()
	UGMEndTurnWidget* EndTurnWidget;

	/*
	UPROPERTY(EditAnywhere, Category = GameOverlay)
	TSubclassOf<UGMEmptyWidget> EmptyWidgetClass;

	UPROPERTY()
	UGMEmptyWidget* EmptyWidget;*/

};