// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackSimple.h"

UURRGA_AttackSimple::UURRGA_AttackSimple()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackSimple::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BP_ApplyGameplayEffectToTarget(TriggerEventData->TargetData, AttackDamageEffect);

}