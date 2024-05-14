// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRAugmentWidget.h"
#include "Components/ListView.h"
#include "Data/URRAugmentData.h"
#include "Framework/URRWaveManager.h"
#include "URR.h"

void UURRAugmentWidget::AddItem(FAugment* augment)
{
	UURRAugmentData* NewItem = NewObject<UURRAugmentData>();
	NewItem->Title = augment->Title;
	NewItem->Desc = augment->Desc;
	NewItem->Icon = augment->Icon;
	NewItem->GE = augment->GE;
	NewItem->Targets = augment->Targets;

	LVAugment->AddItem(NewItem);
}

void UURRAugmentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LVAugment->OnItemClicked().AddUObject(this, &UURRAugmentWidget::ItemClickCallback);
}

void UURRAugmentWidget::ItemClickCallback(UObject* item)
{
	UURRAugmentData* Data = Cast<UURRAugmentData>(item);

	if (Data)
	{
		URR_LOG(LogURR, Log, TEXT("Success"));

		//증강 적용
		//GE를 전달 -> Board에서는 전달받은 GE를 적용 
		//GE는 Rank -> 능력치 조정
		UURRAugmentData* SelectedAugment = Cast<UURRAugmentData>(item);
		if (SelectedAugment)
		{
			OnAugmentSelected.Broadcast(SelectedAugment->GE, SelectedAugment->Targets);
		}

		RemoveFromParent();
	}
}