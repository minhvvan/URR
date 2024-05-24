// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "URRStageClearWidget.generated.h"


UCLASS()
class URR_API UURRStageClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UButton> btnExit;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PopUp;


protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	void OnExitClicked();
};
