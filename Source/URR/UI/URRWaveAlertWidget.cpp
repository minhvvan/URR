// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRWaveAlertWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "URR.h"

void UURRWaveAlertWidget::SetWaveNum(int num)
{
	TxtWave->SetText(FText::AsNumber(num));
}

void UURRWaveAlertWidget::SetMonsterInfo(FMonsterInfo monsters)
{
	//TODO: Img set
	TxtMonster->SetText(FText::AsNumber(monsters.MonsterNum));
}

void UURRWaveAlertWidget::PlayAlertAnim()
{
	EndDelegate.Clear();
	if (!EndDelegate.IsBound())
	{
		EndDelegate.BindDynamic(this, &UURRWaveAlertWidget::AnimationFinished);
		BindToAnimationFinished(WaveAnim, EndDelegate);
	}

	if (MonsterAnim) PlayAnimation(MonsterAnim);
	if (WaveAnim) PlayAnimation(WaveAnim);
}

void UURRWaveAlertWidget::AnimationFinished()
{
	URR_LOG(LogURR, Log, TEXT("AnimationFinished"));
	UnbindFromAnimationFinished(WaveAnim, EndDelegate);
	OnAnimFinish.Broadcast();
}
