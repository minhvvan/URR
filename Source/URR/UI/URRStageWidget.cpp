// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRStageWidget.h"
#include "Animation/WidgetAnimation.h"
#include "URR.h"

void UURRStageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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

void UURRStageWidget::DetachWidget()
{
	if (BoderShow) 
	{
		EndDelegate.BindDynamic(this, &UURRStageWidget::AnimationFinished);
		BindToAnimationFinished(BoderShow, EndDelegate);

		PlayAnimationReverse(BoderShow);
	}
}
