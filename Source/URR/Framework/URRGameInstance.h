// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "URRGameInstance.generated.h"


UENUM(BlueprintType)
enum class EUnitType : uint8 
{ 
	UT_Pistol UMETA(DisplayName = "Pistol"), 
	UT_Rifle UMETA(DisplayName = "Rifle"),
	UT_Mine UMETA(DisplayName = "Mine"),
	UT_Rocket UMETA(DisplayName = "Rocket"),
	UT_MechrPistol UMETA(DisplayName = "MechaPistol"),
	UT_MechaRifle UMETA(DisplayName = "MechaRifle"),
	UT_MechaShotGun UMETA(DisplayName = "MechaShotGun"),
	UT_MechaSniper UMETA(DisplayName = "MechaSniper"),
};


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


UCLASS()
class URR_API UURRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UURRGameInstance();

	TArray<FMonsterInfo> GetWaveInfo(int stageNum);

protected:
	UPROPERTY()
	TObjectPtr<UDataTable> StageWaveInfo;
};
