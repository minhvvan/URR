// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_RankUp_Min.h"
#include "Character/URRBoard.h"
#include "Character/URRCharacterUnit.h"
#include "Actor/URRTile.h"

UURRGA_RankUp_Min::UURRGA_RankUp_Min()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_RankUp_Min::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRBoard* Board = Cast<AURRBoard>(ActorInfo->AvatarActor);
	if (!Board) return;

	auto Tiles = Board->GetTiles();
	if (Tiles.Num() == 0) return;

	AURRTile* tempMin = nullptr;

	for (const auto& row : Tiles)
	{
		for (const auto& tile : row)
		{
			if (tile->IsEmpty()) continue;

			if (!tempMin)
			{
				tempMin = tile;
				continue;
			}

			if (tempMin->GetRank() > tile->GetRank())
			{
				tempMin = tile;
			}
		}
	}

	if (tempMin)
	{
		tempMin->RankUpUnit();
	}
}
