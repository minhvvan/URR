// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackSpwanActor.h"
#include "AbilitySystemBlueprintLibrary.h"

UURRGA_AttackSpwanActor::UURRGA_AttackSpwanActor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackSpwanActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TriggerEventData->TargetData, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);

		FVector TargetLoc = HitResult.GetActor()->GetActorLocation();
		//actor spawn
	}
}