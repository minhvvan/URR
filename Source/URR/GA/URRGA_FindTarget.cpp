// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_FindTarget.h"
#include "GA/AT/URRAT_FindTarget.h"

UURRGA_FindTarget::UURRGA_FindTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_FindTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//AT »ý¼º
	UURRAT_FindTarget* FindTargetAT = UURRAT_FindTarget::FindTarget(this, TEXT("FindTarget"), TargetActorClass, AttackDamageEffect);
	FindTargetAT->ReadyForActivation();
}