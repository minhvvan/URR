// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRAugmentItemWidget.h"
#include "Data/URRAugmentData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UURRAugmentItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UURRAugmentData* Data = Cast<UURRAugmentData>(ListItemObject);
	if (!Data) return;

	ImgIcon->SetBrushFromTexture(Data->Icon);
	TxtTitle->SetText(FText::FromString(Data->Title));
	TxtDesc->SetText(FText::FromString(Data->Desc));
}

void UURRAugmentItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}