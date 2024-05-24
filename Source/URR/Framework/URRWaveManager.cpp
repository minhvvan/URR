// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/URRWaveManager.h"
#include "URR.h"
#include "Kismet/GameplayStatics.h"
#include "Data/URRAugmentData.h"
#include "Actor/URRMonsterSpawner.h"
#include "Character/URRBoard.h"
#include "UI/URRAugmentWidget.h"
#include "Kismet/KismetArrayLibrary.h"

UURRWaveManager::UURRWaveManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> StageWaveInfoRef(TEXT("/Script/Engine.DataTable'/Game/URR/Data/DT_StageWaveInfo.DT_StageWaveInfo'"));
	if (StageWaveInfoRef.Succeeded())
	{
		StageWaveInfo = StageWaveInfoRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> AugmentRef(TEXT("/Script/Engine.DataTable'/Game/URR/Data/DT_Augment.DT_Augment'"));
	if (AugmentRef.Succeeded())
	{
		AugmentInfo = AugmentRef.Object;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> AugmentWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/URR/UI/WBP_Augment.WBP_Augment_C'"));
	if (AugmentWidgetRef.Succeeded())
	{
		AugmentWidgetClass = AugmentWidgetRef.Class;
	}
}

TArray<FMonsterInfo> UURRWaveManager::GetWaveInfo(int stageNum)
{
	if (!StageWaveInfo) return TArray<FMonsterInfo>();

	FStageWaveInfo* Result = StageWaveInfo->FindRow<FStageWaveInfo>(FName(FString::Printf(TEXT("%d"), stageNum)), TEXT(""));
	if (!Result) return TArray<FMonsterInfo>();

	return Result->Waves;
}

void UURRWaveManager::SetUpAllAugment()
{
	if (!AugmentInfo) return;
	AugmentInfo->GetAllRows<FAugment>(TEXT("GetAllRows"), Augments);
}

void UURRWaveManager::PrepareNextWave()
{
	UGameplayStatics::SetGamePaused(GEngine->GameViewport->GetWorld(), true);
	if (Augments.Num() == 0) SetUpAllAugment();

	AugmentWidget = CreateWidget<UURRAugmentWidget>(GEngine->GameViewport->GetWorld(), AugmentWidgetClass);
	if (AugmentWidget)
	{
		AugmentWidget->OnAugmentSelected.AddDynamic(this, &UURRWaveManager::AugmentSelectedCallback);
		AugmentWidget->AddToViewport();

		for (int i = 0; i < 3; i++)
		{
			int idx = FMath::RandRange(0, Augments.Num()-1);
			FAugment* augment = Augments[idx];
			Augments.Remove(augment);
			CurrentShowAugments.Add(augment);

			AugmentWidget->AddItem(augment, i);
		}
	}
}

void UURRWaveManager::FinishStage()
{
	//move to Lobby
	URR_LOG(LogURR, Log, TEXT("Stage Clear"));
}

TArray<FAugment*> UURRWaveManager::GetAugments(int Rank)
{
	if(SelectedAugment.Contains(Rank)) return SelectedAugment[Rank];

	return TArray<FAugment*>();
}

void UURRWaveManager::StartNextWave()
{
	UGameplayStatics::SetGamePaused(GEngine->GameViewport->GetWorld(), false);

	if(!Spawner) Spawner = CastChecked<AURRMonsterSpawner>(UGameplayStatics::GetActorOfClass(GEngine->GameViewport->GetWorld(), AURRMonsterSpawner::StaticClass()));
	Spawner->SpawnMonster();
}

void UURRWaveManager::AugmentSelectedCallback(UURRAugmentData* augment)
{
	UGameplayStatics::SetGamePaused(GEngine->GameViewport->GetWorld(), false);
	AURRBoard* Board = Cast<AURRBoard>(UGameplayStatics::GetActorOfClass(GEngine->GameViewport->GetWorld(), AURRBoard::StaticClass()));

	if (Board)
	{
		EAugmentType type = augment->AugmentData->AugmentType;

		if (type == EAugmentType::AUG_Unit)
		{
			for (auto rank : augment->AugmentData->Targets)
			{
				if(!SelectedAugment.Contains(rank)) SelectedAugment.Add(rank);
				SelectedAugment[rank].Add(augment->AugmentData);
			}

			Board->ApplyAugmentToUnit(augment->AugmentData->GE, augment->AugmentData->Targets);
		}
		else if (type == EAugmentType::AUG_Util)
		{
			Board->ApplyAugmentToSelf(augment->AugmentData->GE);
		}
	}

	CurrentShowAugments.Remove(augment->AugmentData);
	while (!CurrentShowAugments.IsEmpty())
	{
		Augments.Add(CurrentShowAugments.Pop());
	}

	StartNextWave();
}