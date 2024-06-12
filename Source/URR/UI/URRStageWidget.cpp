// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRStageWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ListView.h"
#include "Animation/WidgetAnimation.h"
#include "Data/URRMonsterData.h"
#include "Framework/URRWaveManager.h"
#include "Framework/URRLobbyGameMode.h"
#include "URR.h"

void UURRStageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BtnPlay->OnClicked.AddDynamic(this, &UURRStageWidget::OnPlayClicked);
}

void UURRStageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TxtStageNum->SetText(FText::AsNumber(stageNum));

	if (stageScore >= 20) ImgStar1->SetBrushFromTexture(FillStar);
	if (stageScore >= 50) ImgStar2->SetBrushFromTexture(FillStar);
	if (stageScore >= 90) ImgStar3->SetBrushFromTexture(FillStar);

	if (stageCompleted) TxtPlay->SetText(FText::FromString(TEXT("Replay")));

	UURRWaveManager* WaveManager = Cast<UURRWaveManager>(GEngine->GameSingleton);
	if (!WaveManager) return;

	Waves = WaveManager->GetWaveInfo(stageNum);
	if (Waves.Num() == 0) return;
	for (auto wave : Waves)
	{
		UURRMonsterData* WaveData = NewObject<UURRMonsterData>();
		WaveData->MonsterInfo = wave;

		LVMonsterInfo->AddItem(WaveData);
	}

	if (stageCompleted || bCurrentStage)
	{
		BorderLock->SetVisibility(ESlateVisibility::Hidden);
		BtnPlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BorderLock->SetVisibility(ESlateVisibility::Visible);
		BtnPlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (BoderShow) PlayAnimation(BoderShow);
}

void UURRStageWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UURRStageWidget::AnimationFinished()
{
	RemoveFromViewport();
}

void UURRStageWidget::OnPlayClicked()
{
	AURRLobbyGameMode* GM = Cast<AURRLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;

	GM->OpenStageLevel(stageNum);
	DetachWidget();
}

void UURRStageWidget::DetachWidget()
{
	if (BoderShow) 
	{
		EndDelegate.BindDynamic(this, &UURRStageWidget::AnimationFinished);
		BindToAnimationFinished(BoderShow, EndDelegate);

		PlayAnimationReverse(BoderShow);
	}
}
