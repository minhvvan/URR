// Fill out your copyright notice in the Description page of Project Settings.


#include "URRGA_SpawnUnit.h"
#include "GA/AT/URRAT_SpawnUnit.h"
#include "URR.h"

UURRGA_SpawnUnit::UURRGA_SpawnUnit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_SpawnUnit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//at생성 & callback 설정
	UURRAT_SpawnUnit* SpawnAT = UURRAT_SpawnUnit::SpawnUnit(this, FName(TEXT("SpawnUnit")));
	SpawnAT->OnCompeleteSpawnUnit.AddDynamic(this, &UURRGA_SpawnUnit::SpawnCompleteCallback);
	SpawnAT->ReadyForActivation();

	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);	
}

void UURRGA_SpawnUnit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_SpawnUnit::SpawnCompleteCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
