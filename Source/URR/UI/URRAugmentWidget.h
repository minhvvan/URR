// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "URRAugmentWidget.generated.h"

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
	void AddItem(struct FAugment* augment);

public:
	virtual void NativeOnInitialized();

protected:
	void ItemClickCallback(UObject* item);
};
