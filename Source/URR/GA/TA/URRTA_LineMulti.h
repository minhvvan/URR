// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/URRTA_Trace.h"
#include "URRTA_LineMulti.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRTA_LineMulti : public AURRTA_Trace
{
	GENERATED_BODY()

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
};