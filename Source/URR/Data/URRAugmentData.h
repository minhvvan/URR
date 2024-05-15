// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayEffect.h"
#include "Framework/URRWaveManager.h"
#include "URRAugmentData.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRAugmentData : public UObject
{
	GENERATED_BODY()
	
public:
	FAugment* AugmentData;
};
