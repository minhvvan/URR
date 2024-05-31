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
	TObjectPtr<class UListView> LVMonsterInfo;

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

protected:
	FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(EditAnywhere, Category = Monster)
	int stageNum;

	TArray<struct FMonsterInfo> Waves;
};
