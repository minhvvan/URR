// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "URRGA_RankUp_All.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_RankUp_All : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UURRGA_RankUp_All();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
