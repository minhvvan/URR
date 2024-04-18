// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "URRGA_MoveToGate.generated.h"

/**
 * 
 */
UCLASS(config=URR)
class URR_API UURRGA_MoveToGate : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UURRGA_MoveToGate();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void GateReachedCallback();

	FVector GatePos;
};
