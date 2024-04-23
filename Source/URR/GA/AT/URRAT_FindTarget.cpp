// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/URRAT_FindTarget.h"
#include "GA/TA/URRTA_FirstSingle.h"
#include "GA/TA/URRTA_Trace.h"
#include "AbilitySystemComponent.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"

UURRAT_FindTarget::UURRAT_FindTarget()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UURRAT_FindTarget* UURRAT_FindTarget::FindTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<class AURRTA_Trace> TargetActorClass)
{
	UURRAT_FindTarget* NewTask = NewAbilityTask<UURRAT_FindTarget>(OwningAbility, TaskInstanceName);
	NewTask->TAClass = TargetActorClass;
	return NewTask;
}

void UURRAT_FindTarget::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}

void UURRAT_FindTarget::Activate()
{
	Super::Activate();
}

void UURRAT_FindTarget::TickTask(float DeltaTime)
{
	if (!TAClass) return;

	Super::TickTask(DeltaTime);

	//Make Target
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();
}

void UURRAT_FindTarget::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

void UURRAT_FindTarget::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AURRTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AURRTA_Trace>(TAClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UURRAT_FindTarget::OnTargetDataReadyCallback);
		SpawnedTargetActor->SetShowDebug(true);
	}
}

void UURRAT_FindTarget::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UURRAT_FindTarget::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHnadle)
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		FGameplayTagContainer Tag;
		Tag.AddTag(URRTAG_UNIT_ATTACK);
		ASC->TryActivateAbilitiesByTag(Tag);
	}
}