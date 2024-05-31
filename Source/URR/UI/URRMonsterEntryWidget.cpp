// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRMonsterEntryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/URRWaveManager.h"
#include "Data/URRMonsterData.h"

void UURRMonsterEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UURRMonsterData* Data = Cast<UURRMonsterData>(ListItemObject);
	if (!Data) return;

	SetMonsterInfo(Data->MonsterInfo);
}

void UURRMonsterEntryWidget::SetMonsterInfo(const FMonsterInfo& monsters)
{
	ImgMonster->SetBrushFromTexture(MonsterImages[monsters.MonsterID]);
	TxtMonster->SetText(FText::AsNumber(monsters.MonsterNum));
}