// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/URRGA_Attack.h"
#include "URRGA_AttackSpwanActor.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_AttackSpwanActor : public UURRGA_Attack
{
	GENERATED_BODY()

public:
	UURRGA_AttackSpwanActor();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatGE;
};