// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_UnitPlaced.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/URRCharacterUnit.h"

UURRGA_UnitPlaced::UURRGA_UnitPlaced()
{
}

void UURRGA_UnitPlaced::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRCharacterUnit* UnitCharacter = CastChecked<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), UnitCharacter->GetPlacedActionMontage(), 1.f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UURRGA_UnitPlaced::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UURRGA_UnitPlaced::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UURRGA_UnitPlaced::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_UnitPlaced::OnCompleteCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_UnitPlaced::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}