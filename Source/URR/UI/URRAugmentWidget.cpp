// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRAugmentWidget.h"
#include "UI/URRAugmentItemWidget.h"
#include "Components/ListView.h"
#include "Data/URRAugmentData.h"
#include "Framework/URRWaveManager.h"
#include "URR.h"

void UURRAugmentWidget::AddItem(FAugment* augment, int i)
{
	UURRAugmentItemWidget* Target = Augment1;

	switch (i)
	{
	case 0:
		Target = Augment1;
		break;
	case 1:
		Target = Augment2;
		break;
	case 2:
		Target = Augment3;
		break;
	}

	UURRAugmentData* NewItem = NewObject<UURRAugmentData>();
	NewItem->AugmentData = augment;

	Target->SetWidgetInfo(NewItem);
	Target->OnSelected.AddDynamic(this, &UURRAugmentWidget::ItemClickCallback);
}

void UURRAugmentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UURRAugmentWidget::ItemClickCallback(UURRAugmentData* augment)
{
	OnAugmentSelected.Broadcast(augment);
	RemoveFromParent();
}