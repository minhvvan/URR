// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Framework/URRWaveManager.h"
#include "URRMonsterData.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRMonsterData : public UObject
{
	GENERATED_BODY()
	
public:
	FMonsterInfo MonsterInfo;
};
