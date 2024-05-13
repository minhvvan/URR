// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "URRAugmentItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRAugmentItemWidget : public UURRGASWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<class UImage> ImgIcon;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtTitle;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtDesc;

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnInitialized();

protected:

};
