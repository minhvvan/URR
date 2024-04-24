// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/URRAT_FindTarget.h"
#include "GA/TA/URRTA_FirstSingle.h"
#include "GA/TA/URRTA_Trace.h"
#include "Character/URRCharacterMonster.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"

UURRAT_FindTarget::UURRAT_FindTarget()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UURRAT_FindTarget* UURRAT_FindTarget::FindTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<class AURRTA_Trace> TargetActorClass, TSubclassOf<UGameplayEffect> AttackDamageEffect)
{
	UURRAT_FindTarget* NewTask = NewAbilityTask<UURRAT_FindTarget>(OwningAbility, TaskInstanceName);
	NewTask->TAClass = TargetActorClass;
	NewTask->AttackDamageEffect = AttackDamageEffect;
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

void UURRAT_FindTarget::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(DataHandle, 0))
	{
		UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
		if (ASC)
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle, 0);
			AURRCharacterMonster* TargetMonster = CastChecked<AURRCharacterMonster>(HitResult.GetActor());
			AActor* Unit = ASC->GetAvatarActor();

			if (Unit && TargetMonster)
			{
				FVector TargetPos = TargetMonster->GetActorLocation();

				FRotator NewRot = Unit->GetActorRotation();
				NewRot.Yaw = UKismetMathLibrary::FindLookAtRotation(Unit->GetActorLocation(), TargetPos).Yaw;

				Unit->SetActorRotation(NewRot);

				URR_LOG(LogURR, Log, TEXT("Find Target"));
				FGameplayEventData PayloadData;
				PayloadData.TargetData = DataHandle;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Unit, URRTAG_UNIT_ATTACK, PayloadData);
			}
		}
	}
	else
	{
	}
}