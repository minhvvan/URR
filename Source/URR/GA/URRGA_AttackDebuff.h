// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/URRGA_Attack.h"
#include "URRGA_AttackDebuff.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_AttackDebuff : public UURRGA_Attack
{
	GENERATED_BODY()
	
public:
	UURRGA_AttackDebuff();

protected:
	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<UGameplayEffect> DebuffEffect;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
