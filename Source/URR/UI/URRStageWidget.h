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
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BoderShow;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeDestruct();

	FWidgetAnimationDynamicEvent EndDelegate;

	UFUNCTION()
	void AnimationFinished();
public:
	void DetachWidget();
};
