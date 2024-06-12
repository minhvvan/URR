// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackSpwanActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/URRCharacterUnit.h"
#include "Character/URRCharacterMonster.h"
#include "Attribute/URRUnitAttributeSet.h"
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

		AURRCharacterUnit* Unit = CastChecked<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());
		AURRCharacterMonster* TargetMonster = Unit->GetTargetMonster();
		TSubclassOf<AURRProjectile> Projectileclass = Unit->GetProjectileClass();
		if (!Projectileclass || !TargetMonster) return;

		AURRProjectile* Projectile = GetWorld()->SpawnActorDeferred<AURRProjectile>(Projectileclass, FTransform::Identity, Unit, Unit, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		//Projectile StatÃÊ±âÈ­
		UAbilitySystemComponent* SourceASC = Unit->GetAbilitySystemComponent();
		if (!SourceASC) return;

		const UURRUnitAttributeSet* UnitAttribute = SourceASC->GetSet<UURRUnitAttributeSet>();
		if (!UnitAttribute) return;

		Projectile->SetAttackRate(UnitAttribute->GetAttackRate());
		Projectile->SetKnockBackDist(UnitAttribute->GetKnockBackDist());
		Projectile->SetSlowRate(UnitAttribute->GetSlowRate());
		Projectile->SetCriticalProb(UnitAttribute->GetCriticalProbability());
		Projectile->SetCriticalRate(UnitAttribute->GetCriticalAttackRate());

		Projectile->FinishSpawning(Unit->GetMuzzleTransform());

		FActorSpawnParameters params;
		FVector SpawnLoc = Projectile->GetActorLocation();
		FVector EndLoc = TargetMonster->GetActorLocation();
		float arcValue = .5f;

		FVector outVelocity;
		if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OUT outVelocity, SpawnLoc, EndLoc, GetWorld()->GetGravityZ(), arcValue))
		{
			Projectile->FireInDirection(outVelocity);
		}
	}
}