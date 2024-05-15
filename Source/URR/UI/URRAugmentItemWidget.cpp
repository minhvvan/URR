// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRAugmentItemWidget.h"
#include "Data/URRAugmentData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Framework/URRWaveManager.h"
#include "URR.h"

void UURRAugmentItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UURRAugmentData* Data = Cast<UURRAugmentData>(ListItemObject);
	if (!Data) return;
}

void UURRAugmentItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//bind button
	BtnAugment->OnClicked.AddDynamic(this, &UURRAugmentItemWidget::OnClicked);
}

void UURRAugmentItemWidget::SetWidgetInfo(UURRAugmentData* augmentData)
{
	AugmentData = augmentData;
	SetIcon(AugmentData->AugmentData->Icon);
	SetTitle(AugmentData->AugmentData->Title);
	SetDesc(AugmentData->AugmentData->Desc);
}

void UURRAugmentItemWidget::OnClicked()
{
	OnSelected.Broadcast(AugmentData);
}

void UURRAugmentItemWidget::SetIcon(UTexture2D* icon)
{
	ImgIcon->SetBrushFromTexture(icon);
}

void UURRAugmentItemWidget::SetTitle(FString title)
{
	TxtTitle->SetText(FText::FromString(title));
}

void UURRAugmentItemWidget::SetDesc(FString desc)
{
	TxtDesc->SetText(FText::FromString(desc));
}
