// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "URRGA_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_FindTarget : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UURRGA_FindTarget();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<class AURRTA_Trace> TargetActorClass;

	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
};
