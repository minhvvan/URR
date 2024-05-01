// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackDebuff.h"
#include "URR.h"

UURRGA_AttackDebuff::UURRGA_AttackDebuff()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackDebuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	BP_ApplyGameplayEffectToTarget(TriggerEventData->TargetData, AttackDamageEffect);
	BP_ApplyGameplayEffectToTarget(TriggerEventData->TargetData, DebuffEffect);
}