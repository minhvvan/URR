// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "URRStageWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRStageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<class UTextBlock> TxtStageNum;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ImgStar1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ImgStar2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ImgStar3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UButton> BtnPlay;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtPlay;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UListView> LVMonsterInfo;	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> BorderLock;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BoderShow;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	UFUNCTION()
	void AnimationFinished();

	UFUNCTION()
	void OnPlayClicked();

public:
	void DetachWidget();

	void SetStageNum(int num) { stageNum = num; }
	void SetStageScore(int score) { stageScore = score; }
	void SetStageCompleted(bool bComplete) { stageCompleted = bComplete; }
	void SetCurrentStage(bool bCurrent) { bCurrentStage = bCurrent; }

protected:
	FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(EditAnywhere, Category = Monster)
	int stageNum;

	UPROPERTY(EditAnywhere, Category = Monster)
	int stageScore;

	UPROPERTY(EditAnywhere, Category = Monster)
	bool stageCompleted;

	UPROPERTY(EditAnywhere, Category = Monster)
	bool bCurrentStage;

	UPROPERTY(EditAnywhere, Category = Stage)
	UTexture2D* FillStar;

	TArray<struct FMonsterInfo> Waves;
};
