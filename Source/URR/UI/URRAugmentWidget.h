// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "Framework/URRWaveManager.h"
#include "URRAugmentWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAugmentSelected, TSubclassOf<UGameplayEffect>, GE, TArray<int>, Targets, EAugmentType, AugmentType);
/**
 * 
 */
UCLASS()
class URR_API UURRAugmentWidget : public UURRGASWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<class UListView> LVAugment;

public:
	FOnAugmentSelected OnAugmentSelected;

public:
	void AddItem(struct FAugment* augment);

public:
	virtual void NativeOnInitialized();

protected:
	void ItemClickCallback(UObject* item);
};
