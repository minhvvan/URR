// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_RankUp_All.h"
#include "Character/URRBoard.h"
#include "Character/URRCharacterUnit.h"
#include "Actor/URRTile.h"

UURRGA_RankUp_All::UURRGA_RankUp_All()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_RankUp_All::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRBoard* Board = Cast<AURRBoard>(ActorInfo->AvatarActor);
	if (!Board) return;

	auto Tiles = Board->GetTiles();
	if (Tiles.Num() == 0) return;

	for (const auto& row : Tiles)
	{
		for (const auto& tile : row)
		{
			if (tile->IsEmpty()) continue;
			tile->RankUpUnit();
		}
	}
}
