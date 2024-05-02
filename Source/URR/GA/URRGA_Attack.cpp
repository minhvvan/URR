// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_Attack.h"
#include "Character/URRCharacterUnit.h"
#include "Character/URRCharacterMonster.h"
#include "GA/AT/URRAT_FindTarget.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/URRCharacterMonster.h"
#include "URR.h"

UURRGA_Attack::UURRGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AURRCharacterUnit* Unit = Cast<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());
	AURRCharacterMonster* Monster = Cast<AURRCharacterMonster>(UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0).GetActor());
	
	if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TriggerEventData->TargetData, 0))
	{
		TArray<AActor*> Monsters = (UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TriggerEventData->TargetData, 0));
		Monster = CastChecked<AURRCharacterMonster>(Monsters[0]);
	}

	if (!Monster) return;

	Unit->SetTargetMonster(Monster);

	UAbilityTask_PlayMontageAndWait* MontageAT = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), Unit->GetAttackMontage(), 1.f);
	MontageAT->OnCompleted.AddDynamic(this, &UURRGA_Attack::OnCompleteCallback);
	MontageAT->OnInterrupted.AddDynamic(this, &UURRGA_Attack::OnInterruptedCallback);
	MontageAT->OnCancelled.AddDynamic(this, &UURRGA_Attack::OnCancelledCallback);

	MontageAT->ReadyForActivation();

	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

void UURRGA_Attack::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorInfo->AvatarActor.Get());

	if (ASC)
	{
		const UURRUnitAttributeSet* Attribute = ASC->GetSet<UURRUnitAttributeSet>();
		if (Attribute)
		{
			FGameplayEffectModifierMagnitude CooldownMagnitude(1.f / Attribute->GetAttackSpeed());
			CooldownGE->DurationMagnitude = CooldownMagnitude;
		}
	}

	if (CooldownGE)
	{
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));
	}
}

void UURRGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_Attack::OnCompleteCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_Attack::OnCancelledCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
