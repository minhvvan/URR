// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRStageClearWidget.h"
#include "Components/Button.h"
#include "Framework/URRWaveManager.h"

void UURRStageClearWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	btnExit->OnClicked.AddDynamic(this, &UURRStageClearWidget::OnExitClicked);
	if (PopUp) PlayAnimation(PopUp);
}

void UURRStageClearWidget::OnExitClicked()
{
	//Lobby¿Ãµø
	UURRWaveManager* WaveManager = Cast<UURRWaveManager>(GEngine->GameSingleton);
	if (!WaveManager) return;

	WaveManager->FinishStage();
	RemoveFromParent();
}
