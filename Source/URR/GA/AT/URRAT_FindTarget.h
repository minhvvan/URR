// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "URRAT_FindTarget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTraceResultDelegate);

/**
* 
 * 
 */
UCLASS()
class URR_API UURRAT_FindTarget : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UURRAT_FindTarget();

	static UURRAT_FindTarget* FindTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<class AURRTA_Trace> TargetActorClass);

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

	void SetShowDebug(bool bShow) { bShowDebug = bShow; }

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UFUNCTION()
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<AURRTA_Trace> TAClass;

	UPROPERTY(EditAnywhere, category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY()
	TObjectPtr<AURRTA_Trace> SpawnedTargetActor;

	bool bShowDebug;
};
