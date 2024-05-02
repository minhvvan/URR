// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/URRTA_LineMulti.h"
#include "Abilities/GameplayAbility.h"
#include "Character/URRCharacterMonster.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "Attribute/URRMonsterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "URR.h"
#include "Physics/URRCollision.h"

FGameplayAbilityTargetDataHandle AURRTA_LineMulti::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC) return FGameplayAbilityTargetDataHandle();

	const UURRUnitAttributeSet* AttrubuteSet = ASC->GetSet<UURRUnitAttributeSet>();
	if (!AttrubuteSet) return FGameplayAbilityTargetDataHandle();

	TArray<FHitResult> InRangeResults;
	const float AttackRange = AttrubuteSet->GetAttackRange();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UURRAttack), false, Character);
	const FVector Center = Character->GetActorLocation();
	FVector End;

	bool bInRange = GetWorld()->SweepMultiByChannel(InRangeResults, Center, Center, FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeSphere(AttackRange), Params);

	TArray<TWeakObjectPtr<AActor>> HittedActors;
	if (bInRange)
	{
		AURRCharacterMonster* TargetMonster = nullptr;
		float MaxDist = 0;

		for (FHitResult& hitResult : InRangeResults)
		{
			//∞°¿Â ∏’ ≈∏±Í
			AURRCharacterMonster* Monster = CastChecked<AURRCharacterMonster>(hitResult.GetActor());
			const UURRMonsterAttributeSet* MonsterAttributeSet = Monster->GetAbilitySystemComponent()->GetSet<UURRMonsterAttributeSet>();

			float dist = Character->GetDistanceTo(Monster);
			if (dist > MaxDist)
			{
				TargetMonster = Monster;
				MaxDist = dist;
			}
		}

		TArray<FHitResult> OutHitResults;

		End = TargetMonster->GetActorLocation();
		bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Center, End, FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeBox(FVector(100, 100, 1000)), Params);
	
		if (bHitDetected)
		{
			for (FHitResult& hitResult : OutHitResults)
			{
				AURRCharacterMonster* Monster = CastChecked<AURRCharacterMonster>(hitResult.GetActor());
				HittedActors.Add(Monster);
			}
		}
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();
	TargetData->SetActors(HittedActors);

	DataHandle.Add(TargetData);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Center;
		FColor DrawColor = bInRange ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), CapsuleOrigin, AttackRange, 32, DrawColor, false, 1.f);
	}

#endif

	return DataHandle;
}
