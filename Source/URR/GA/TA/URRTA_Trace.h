// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "URRTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AURRTA_Trace();

	virtual void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; };

protected:
	bool bShowDebug;
};