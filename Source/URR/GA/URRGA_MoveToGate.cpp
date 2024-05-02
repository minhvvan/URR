// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_MoveToGate.h"
#include "Character/URRCharacterMonster.h"
#include "GA/AT/URRAT_MoveWithSpline.h"
#include "Curves/CurveVector.h"
#include "URR.h"

UURRGA_MoveToGate::UURRGA_MoveToGate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_MoveToGate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UURRAT_MoveWithSpline* MoveAT = UURRAT_MoveWithSpline::MoveToLocation(this, TEXT("MoveToGate"));
	MoveAT->OnTargetLocationReached.AddDynamic(this, &UURRGA_MoveToGate::GateReachedCallback);

	MoveAT->ReadyForActivation();
}

void UURRGA_MoveToGate::GateReachedCallback()
{
	AURRCharacterMonster* Monster = Cast<AURRCharacterMonster>(CurrentActorInfo->AvatarActor.Get());
	Monster->SetbMove(false);
	Monster->StartAttack();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}