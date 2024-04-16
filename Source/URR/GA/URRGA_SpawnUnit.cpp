// Fill out your copyright notice in the Description page of Project Settings.


#include "URRGA_SpawnUnit.h"
#include "Character/URRBoard.h"
#include "Actor/URRTile.h"
#include "URR.h"

UURRGA_SpawnUnit::UURRGA_SpawnUnit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_SpawnUnit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRBoard* Board = Cast<AURRBoard>(ActorInfo->AvatarActor.Get());
	AURRTile* Tile = Board->GetEmptyTile();
	if (Tile)
	{
		int temp = FMath::Rand() % 100;
		int rank = 0;
		if (temp < 70) rank = 7;
		else if (temp < 90) rank = 8;
		else if (temp < 98) rank = 2;
		else rank = 3;

		Tile->SpawnUnit(rank);

		CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_SpawnUnit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}