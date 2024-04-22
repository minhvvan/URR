// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/URRTA_Trace.h"
#include "URRTA_FirstSingle.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRTA_FirstSingle : public AURRTA_Trace
{
	GENERATED_BODY()
	
public:
	AURRTA_FirstSingle();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
};
