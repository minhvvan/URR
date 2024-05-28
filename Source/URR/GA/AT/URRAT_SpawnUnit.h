// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "URRAT_SpawnUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompleteSpawnDelegate);

/**
 * 
 */
UCLASS()
class URR_API UURRAT_SpawnUnit : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	static UURRAT_SpawnUnit* SpawnUnit(UGameplayAbility* OwningAbility, FName TaskInstanceName);
	virtual void Activate() override;

	FCompleteSpawnDelegate OnCompeleteSpawnUnit;

protected:
	UFUNCTION()
	void CompleteSpawnUnitCallback();
};
