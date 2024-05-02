// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "URRGA_SmashGate.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_SmashGate : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UURRGA_SmashGate();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	virtual void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnCancelledCallback();

	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<class UGameplayEffect> SmashGateEffect;
};