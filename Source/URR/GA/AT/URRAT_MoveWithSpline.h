// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "URRAT_MoveWithSpline.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveToLocationDelegate);

/**
 * 
 */
UCLASS()
class URR_API UURRAT_MoveWithSpline : public UAbilityTask
{
	GENERATED_BODY()

public:
	UURRAT_MoveWithSpline();

	UPROPERTY(BlueprintAssignable)
	FMoveToLocationDelegate OnTargetLocationReached;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UURRAT_MoveWithSpline* MoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location);

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	
	virtual void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

public:
	bool bIsFinished;
	FVector GatePos;
};
