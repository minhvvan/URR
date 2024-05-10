// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRAugmentWidget.h"
#include "Components/ListView.h"
#include "Data/URRAugmentData.h"
#include "Framework/URRWaveManager.h"

void UURRAugmentWidget::AddItem(FAugment* augment)
{
	UURRAugmentData* NewItem = NewObject<UURRAugmentData>();
	NewItem->Title = augment->Title;
	NewItem->Desc = augment->Desc;
	NewItem->Icon = augment->Icon;

	LVAugment->AddItem(NewItem);
}