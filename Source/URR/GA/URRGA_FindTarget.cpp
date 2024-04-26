// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_FindTarget.h"
#include "GA/AT/URRAT_FindTarget.h"
#include "URR.h"

UURRGA_FindTarget::UURRGA_FindTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_FindTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UURRAT_FindTarget* FindTargetAT = UURRAT_FindTarget::FindTarget(this, TEXT("FindTarget"), TargetActorClass);
	FindTargetAT->OnComplete.AddDynamic(this, &UURRGA_FindTarget::AbilityTaskCompleteCallback);
	FindTargetAT->ReadyForActivation();
}

void UURRGA_FindTarget::AbilityTaskCompleteCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
