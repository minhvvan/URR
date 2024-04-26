// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/URRTA_Trace.h"
#include "Abilities/GameplayAbility.h"

AURRTA_Trace::AURRTA_Trace()
{
}

void AURRTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AURRTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AURRTA_Trace::MakeTargetData() const
{
	return FGameplayAbilityTargetDataHandle();
}
