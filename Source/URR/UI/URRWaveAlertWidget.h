// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/URRWaveManager.h"
#include "URRWaveAlertWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimFinished);

UCLASS()
class URR_API UURRWaveAlertWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ImgMonster;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMonster;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtWave;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> MonsterAnim;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WaveAnim;

public:
	void SetWaveNum(int num);
	
	void SetMonsterInfo(FMonsterInfo monsters);

	void PlayAlertAnim();

	FOnAnimFinished OnAnimFinish;

protected:
	FWidgetAnimationDynamicEvent EndDelegate;

	UFUNCTION()
	void AnimationFinished();
};
