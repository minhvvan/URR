// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackDebuff.h"
#include "AbilitySystemComponent.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"

UURRGA_AttackDebuff::UURRGA_AttackDebuff()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackDebuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UURRGA_AttackDebuff::OnCompleteCallback()
{
	BP_ApplyGameplayEffectToTarget(CurrentEventData.TargetData, DebuffEffect);
	BP_ApplyGameplayEffectToTarget(CurrentEventData.TargetData, AttackDamageEffect);

	Super::OnCompleteCallback();
}