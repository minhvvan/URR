// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "URRWaveManager.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MonsterID;

	UPROPERTY(EditAnywhere)
	int MonsterNum;

	UPROPERTY(EditAnywhere)
	int MonsterSpawnTerm;
};

USTRUCT()
struct FStageWaveInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMonsterInfo> Waves;
};

USTRUCT()
struct FAugment : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};


UCLASS()
class URR_API UURRWaveManager : public UObject
{
	GENERATED_BODY()
	
public:
	UURRWaveManager();

public:
	TArray<FMonsterInfo> GetWaveInfo(int stageNum);
	void SetUpAllAugment();
	void PrepareNextWave();

protected:
	void StartNextWave();

protected:
	UPROPERTY()
	TObjectPtr<UDataTable> StageWaveInfo;

	UPROPERTY()
	TObjectPtr<UDataTable> AugmentInfo;

	TObjectPtr<class AURRMonsterSpawner> Spawner;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AugmentWidgetClass;

	TObjectPtr<class UURRAugmentWidget> AugmentWidget;

	TArray<FAugment*> Augments;
};
