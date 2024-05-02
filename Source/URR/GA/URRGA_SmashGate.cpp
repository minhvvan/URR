// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_SmashGate.h"
#include "Character/URRCharacterMonster.h"
#include "Character/URRBoard.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "URR.h"

UURRGA_SmashGate::UURRGA_SmashGate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_SmashGate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AURRCharacterMonster* Monster = Cast<AURRCharacterMonster>(ActorInfo->AvatarActor.Get());
	if (!Monster) return;

	UAbilityTask_PlayMontageAndWait* MontageAT = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), Monster->GetAttackMontage(), 1.f);
	MontageAT->OnCompleted.AddDynamic(this, &UURRGA_SmashGate::OnCompleteCallback);
	MontageAT->OnInterrupted.AddDynamic(this, &UURRGA_SmashGate::OnInterruptedCallback);
	MontageAT->OnCancelled.AddDynamic(this, &UURRGA_SmashGate::OnCancelledCallback);

	MontageAT->ReadyForActivation();
}

void UURRGA_SmashGate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_SmashGate::OnCompleteCallback()
{
	AURRBoard* Board =  Cast<AURRBoard>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Board)
	{
		UAbilitySystemComponent* PlayerASC = Board->GetAbilitySystemComponent();
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(SmashGateEffect, 0, Context);
		if (Spec.IsValid())
		{
			URR_LOG(LogURR, Log, TEXT("GE"));
			ASC->BP_ApplyGameplayEffectSpecToTarget(Spec, PlayerASC);
			CurrentActorInfo->AvatarActor.Get()->Destroy();
		}
	}

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
void UURRGA_SmashGate::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UURRGA_SmashGate::OnCancelledCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
