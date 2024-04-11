// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHudWidget.h"
#include "Components/Button.h"
#include "Character/URRBoard.h"

void UURRHudWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BtnSpawn->OnClicked.AddDynamic(this, &UURRHudWidget::OnSpawnClicked);
}

void UURRHudWidget::OnSpawnClicked()
{
	if (Board.Get() == nullptr)
	{
		Board = CastChecked<AURRBoard>(GetOwningPlayerPawn());
	}

	Board->SpawnUnit();
}