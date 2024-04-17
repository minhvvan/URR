// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRTileRankWidget.h"
#include "Components/TextBlock.h"

void UURRTileRankWidget::SetRank(int rank)
{
	FText text;
	if (rank == 10) text = FText::FromString(TEXT("1024"));
	else text = FText::AsNumber(int(FMath::Pow(2.f, rank)));

	TxtRank->SetText(text);

	if (rank > 6) TxtRank->SetColorAndOpacity(FColor::White);
	else TxtRank->SetColorAndOpacity(FColor::Black);
}