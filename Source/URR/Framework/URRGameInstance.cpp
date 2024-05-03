// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/URRGameInstance.h"

UURRGameInstance::UURRGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> StageWaveInfoRef(TEXT("/Script/Engine.DataTable'/Game/URR/Data/DT_StageWaveInfo.DT_StageWaveInfo'"));
	if (StageWaveInfoRef.Succeeded())
	{
		StageWaveInfo = StageWaveInfoRef.Object;
	}
}

TArray<FMonsterInfo> UURRGameInstance::GetWaveInfo(int stageNum)
{
	if(!StageWaveInfo) return TArray<FMonsterInfo>();

	FStageWaveInfo* Result = StageWaveInfo->FindRow<FStageWaveInfo>(FName(FString::Printf(TEXT("%d"), stageNum)), TEXT(""));
	if (!Result) return TArray<FMonsterInfo>();

	return Result->Waves;
}
