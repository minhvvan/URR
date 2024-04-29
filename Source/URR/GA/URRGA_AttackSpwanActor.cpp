// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackSpwanActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/URRCharacterUnit.h"
#include "Character/URRCharacterMonster.h"
#include "Actor/URRProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"

UURRGA_AttackSpwanActor::UURRGA_AttackSpwanActor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackSpwanActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TriggerEventData->TargetData, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);

		FVector TargetLoc = HitResult.GetActor()->GetActorLocation();
		//actor spawn

		AURRCharacterUnit* Unit = CastChecked<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());
		AURRCharacterMonster* TargetMonster = Unit->GetTargetMonster();
		TSubclassOf<AURRProjectile> Projectileclass = Unit->GetProjectileClass();
		if (!Projectileclass || !TargetMonster) return;

		FActorSpawnParameters params;
		FVector SpawnLoc = Unit->GetActorLocation();
		FVector EndLoc = TargetMonster->GetActorLocation();
		float arcValue = .5f;

		AURRProjectile* Projectile = GetWorld()->SpawnActor<AURRProjectile>(Projectileclass, SpawnLoc, FRotator::ZeroRotator, params);
		if (!Projectile) return;

		//Projectile StatÃÊ±âÈ­
		UAbilitySystemComponent* SourceASC = Unit->GetAbilitySystemComponent();
		UAbilitySystemComponent* TargetASC = Projectile->GetAbilitySystemComponent();
		if (!SourceASC || !TargetASC) return;

		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(InitStatEffect, 0, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(URRTAG_DATA_KNOCKBACK, 200.f);
			SourceASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, TargetASC);
		}

		FVector outVelocity;
		if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OUT outVelocity, SpawnLoc, EndLoc, GetWorld()->GetGravityZ(), arcValue))
		{
			Projectile->FireInDirection(outVelocity);
		}
	}
}