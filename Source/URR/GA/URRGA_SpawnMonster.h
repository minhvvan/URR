// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "URRGA_SpawnMonster.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRGA_SpawnMonster : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UURRGA_SpawnMonster();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Unit, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AURRCharacterMonster> MonsterClass;

	FTimerHandle MonsterSpawnTimer;

	TObjectPtr<class AURRMonsterSpawner> Spawner;
	
	int MonsterID;
	int MonsterNum;

	float MonsterSpawnTerm;

	void StartSpawnMonster();
	void CheckSpawnMonster();
};
