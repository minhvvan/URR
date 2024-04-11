// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "URRHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UButton* BtnSpawn;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnSpawnClicked();

protected:
	TObjectPtr<class AURRBoard> Board;
};
