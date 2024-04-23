// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_Attack.h"
#include "Character/URRCharacterUnit.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "URR.h"

UURRGA_Attack::UURRGA_Attack()
{
}

void UURRGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRCharacterUnit* Unit = CastChecked<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* MontageAT = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), Unit->GetAttackMontage(), 1.f);
	MontageAT->OnCompleted.AddDynamic(this, &UURRGA_Attack::OnCompleteCallback);
	MontageAT->OnInterrupted.AddDynamic(this, &UURRGA_Attack::OnInterruptedCallback);

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