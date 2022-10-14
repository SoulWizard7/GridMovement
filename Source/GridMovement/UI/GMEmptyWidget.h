#pragma once

#include "Blueprint/UserWidget.h"
#include "GMEmptyWidget.generated.h"

class UTextBlock;


UCLASS()
class UGMEmptyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ClickButton();	
};