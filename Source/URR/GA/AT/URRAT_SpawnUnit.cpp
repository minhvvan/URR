// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/URRAT_SpawnUnit.h"
#include "Character/URRBoard.h"
#include "Actor/URRTile.h"
#include "URR.h"

UURRAT_SpawnUnit* UURRAT_SpawnUnit::SpawnUnit(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UURRAT_SpawnUnit* NewTask = NewAbilityTask<UURRAT_SpawnUnit>(OwningAbility, TaskInstanceName);
	return NewTask;
}

void UURRAT_SpawnUnit::Activate()
{
	AURRBoard* Board = Cast<AURRBoard>(Ability->GetAvatarActorFromActorInfo());
	if (!Board) return;

	AURRTile* Tile = Board->GetEmptyTile();
	if (Tile)
	{
		Tile->OnCompleteSpawnUnitDelegate.AddDynamic(this, &UURRAT_SpawnUnit::CompleteSpawnUnitCallback);

		int temp = FMath::Rand() % 100;
		int rank = 0;
		if (temp < 70) rank = 0;
		else if (temp < 90) rank = 1;
		else if (temp < 98) rank = 2;
		else rank = 3;

		rank = 2;

		Tile->SpawnUnit(rank);
	}
	else
	{
		CompleteSpawnUnitCallback();
	}
}

void UURRAT_SpawnUnit::CompleteSpawnUnitCallback()
{
	OnCompeleteSpawnUnit.Broadcast();
	EndTask();
}
