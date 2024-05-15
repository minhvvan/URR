// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "Framework/URRWaveManager.h"
#include "URRAugmentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAugmentSelected, class UURRAugmentData* , AugmentData);
/**
 * 
 */
UCLASS()
class URR_API UURRAugmentWidget : public UURRGASWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<class UURRAugmentItemWidget> Augment1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UURRAugmentItemWidget> Augment2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UURRAugmentItemWidget> Augment3;

public:
	FOnAugmentSelected OnAugmentSelected;

public:
	void AddItem(struct FAugment* augment, int i);

public:
	virtual void NativeOnInitialized();

protected:
	UFUNCTION()
	void ItemClickCallback(class UURRAugmentData* augment);
};
