
#include "GMCombatManager.h"
#include "GridMovement/GMGameMode.h"
#include "GridMovement/UI/GMOverlayWidget.h"
#include "GridMovement/UI/GMPlayerActionWidget.h"
#include "GridMovement/UI/GMEndTurnWidget.h"

AGMCombatManager::AGMCombatManager()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	CombatStart = CreateDefaultSubobject<UGMCombatStart>(TEXT("Combat Start"));
	CombatNewRound = CreateDefaultSubobject<UGMCombatNewRound>(TEXT("Combat NewRound"));
	CombatPlayerTurn = CreateDefaultSubobject<UGMCombatPlayerTurn>(TEXT("Combat Player Turn"));
	CombatEnemyTurn = CreateDefaultSubobject<UGMCombatEnemyTurn>(TEXT("Combat Enemy Turn"));
	CombatEnd = CreateDefaultSubobject<UGMCombatEnd>(TEXT("Combat End"));
}

void AGMCombatManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FString msg = UEnum::GetValueAsString(CombatState);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Green, TEXT("Current combat state = ") + msg, true,
									 FVector2D(1.f));

	switch (CombatState)
	{
	case States::StartState:
		CombatStart->CombatStart();
		break;
	case States::NewRound:
		CombatNewRound->CombatNewRound();
		break;
	case States::PlayerTurnStart:
		CombatPlayerTurn->CombatPlayerTurnStart();
		break;
	case States::PlayerTurn:
		CombatPlayerTurn->CombatPlayerTurn();
		break;
	case States::EnemyTurnStart:
		CombatEnemyTurn->CombatEnemyTurnStart();
		break;
	case States::EnemyTurn:
		CombatEnemyTurn->CombatEnemyTurn();
		break;
	case States::EndCombat:
		CombatEnd->CombatEnd();
		break;
	}
	
}

void AGMCombatManager::BeginPlay()
{
	Super::BeginPlay();
	CombatStart->CombatManager = this;
	CombatNewRound->CombatManager = this;
	CombatPlayerTurn->CombatManager = this;
	CombatEnemyTurn->CombatManager = this;
	PlayerUnits.Empty();
	EnemyUnits.Empty();
	CombatState = States::StartState;
	SpawnButtons();
}

void AGMCombatManager::SpawnButtons()
{
	AGMGameMode* GameMode = AGMGameMode::Get(this);
	GameMode->OverlayWidget->AddActionButton(0);
	GameMode->OverlayWidget->AddActionButton(1);
	GameMode->OverlayWidget->AddActionButton(2);
	GameMode->OverlayWidget->AddActionButton(3);

	GameMode->OverlayWidget->ActionWidgets[0]->SetButtonName(FString::Printf(TEXT("test 1")));
	GameMode->OverlayWidget->ActionWidgets[1]->SetButtonName(FString::Printf(TEXT("test 2")));
	GameMode->OverlayWidget->ActionWidgets[2]->SetButtonName(FString::Printf(TEXT("test 3")));
	GameMode->OverlayWidget->ActionWidgets[3]->SetButtonName(FString::Printf(TEXT("test 4")));

	for (int i = 0; i < GameMode->OverlayWidget->ActionWidgets.Num(); ++i)
	{
		GameMode->OverlayWidget->ActionWidgets[i]->CombatManager = this;
	}

	GameMode->OverlayWidget->AddEndTurnButton();
	GameMode->OverlayWidget->EndTurnWidget->CombatManager = this;
}

void AGMCombatManager::DeSpawnButtons()
{
	AGMGameMode* GameMode = AGMGameMode::Get(this);
	GameMode->OverlayWidget->RemoveActionButton(0);
	GameMode->OverlayWidget->RemoveActionButton(1);
	GameMode->OverlayWidget->RemoveActionButton(2);
	GameMode->OverlayWidget->RemoveActionButton(3);
	GameMode->OverlayWidget->RemoveEndTurnButton();
}

void AGMCombatManager::CombatManagerEndEnemyTurn()
{
	CombatEnemyTurn->EndEnemyTurn();
}

void AGMCombatManager::SetState(::States NewState)
{
	CombatState = NewState;
}

void AGMCombatManager::EndPlayerTurn()
{
	CombatPlayerTurn->EndTurn();
}