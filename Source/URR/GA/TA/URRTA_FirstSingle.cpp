// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/URRTA_FirstSingle.h"
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


FGameplayAbilityTargetDataHandle AURRTA_FirstSingle::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC) return FGameplayAbilityTargetDataHandle();

	const UURRUnitAttributeSet* AttrubuteSet = ASC->GetSet<UURRUnitAttributeSet>();
	if (!AttrubuteSet) return FGameplayAbilityTargetDataHandle();

	TArray<FHitResult> OutHitResults;
	const float AttackRange = AttrubuteSet->GetAttackRange();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UURRAT_Trace), false, Character);
	FVector Center = Character->GetActorLocation();
	Center.Z -= 400;

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Center, Center, FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeSphere(AttackRange), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (bHitDetected)
	{
		FHitResult Target;
		float MaxDist = 0;

		for (FHitResult& hitResult : OutHitResults)
		{
			//선두 고르기
			AURRCharacterMonster* Monster = CastChecked<AURRCharacterMonster>(hitResult.GetActor());
			const UURRMonsterAttributeSet* MonsterAttributeSet = Monster->GetAbilitySystemComponent()->GetSet<UURRMonsterAttributeSet>();
		
			float dist = MonsterAttributeSet->GetDistance();
			if (dist > MaxDist)
			{
				Target = hitResult;
				MaxDist = dist;
			}
		}

		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(Target);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Center;
		FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), CapsuleOrigin, AttackRange, 32, DrawColor, false, 5.f);
	}
	else
	{
		//URR_LOG(LogURR, Log, TEXT("No Debug"));
	}

#endif

	return DataHandle;
}
