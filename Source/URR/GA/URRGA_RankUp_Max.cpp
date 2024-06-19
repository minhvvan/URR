// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_RankUp_Max.h"
#include "Character/URRBoard.h"
#include "Character/URRCharacterUnit.h"
#include "Actor/URRTile.h"
#include "URR.h"

UURRGA_RankUp_Max::UURRGA_RankUp_Max()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_RankUp_Max::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	URR_LOG(LogURR, Log, TEXT("GA"));

	AURRBoard* Board = Cast<AURRBoard>(ActorInfo->AvatarActor);
	if (!Board) return;

	const auto Tiles = Board->GetTiles();
	if (Tiles.Num() == 0) return;

	AURRTile* tempMax = nullptr;

	for (const auto& row : Tiles)
	{
		for (const auto& tile : row)
		{
			if (tile->IsEmpty()) continue;

			if (!tempMax)
			{
				tempMax = tile;
				continue;
			}

			if (tempMax->GetRank() < tile->GetRank())
			{
				tempMax = tile;
			}
		}
	}

	if (tempMax)
	{
		tempMax->RankUpUnit();
	}
}