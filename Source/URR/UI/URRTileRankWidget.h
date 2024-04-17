// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "URRTileRankWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRTileRankWidget : public UURRGASWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UTextBlock* TxtRank;
	
public:
	void SetRank(int rank);
};
