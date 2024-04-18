// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_SpawnMonster.h"
#include "Character/URRCharacterMonster.h"
#include "Actor/URRMonsterSpawner.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "URR.h"

UURRGA_SpawnMonster::UURRGA_SpawnMonster()
{
	ConstructorHelpers::FClassFinder<AURRCharacterMonster> MonsterRef(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Monster.BP_Monster_C'"));
	if (MonsterRef.Succeeded())
	{
		MonsterClass = MonsterRef.Class;
	}
}

void UURRGA_SpawnMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Spawner = CastChecked<AURRMonsterSpawner>(ActorInfo->AvatarActor.Get());
	FMonsterInfo CurrentMonsterInfo = Spawner->GetCurrentMonsterInfo();

	//CurrentMonsterInfo.Num만큼 스폰
	MonsterID = CurrentMonsterInfo.MonsterID;
	MonsterNum = CurrentMonsterInfo.MonsterNum;

	StartSpawnMonster();
}

void UURRGA_SpawnMonster::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_SpawnMonster::StartSpawnMonster()
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AURRCharacterMonster* Monster = Spawner->GetWorld()->SpawnActorDeferred<AURRCharacterMonster>(MonsterClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (Monster)
	{
		Monster->InitMonster(MonsterID);
		Monster->FinishSpawning(Spawner->GetActorTransform());
		Monster->StartMove();

		Spawner->AddSpawnedMonster(Monster);
		MonsterNum--;

		Spawner->GetWorldTimerManager().SetTimer(MonsterSpawnTimer, this, &UURRGA_SpawnMonster::CheckSpawnMonster, 1.f, false);
	}
}

void UURRGA_SpawnMonster::CheckSpawnMonster()
{
	MonsterSpawnTimer.Invalidate();
	if (MonsterNum > 0)
	{
		StartSpawnMonster();
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
