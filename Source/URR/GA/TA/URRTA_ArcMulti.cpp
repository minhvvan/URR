// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/URRTA_ArcMulti.h"
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

FGameplayAbilityTargetDataHandle AURRTA_ArcMulti::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC) return FGameplayAbilityTargetDataHandle();

	const UURRUnitAttributeSet* AttrubuteSet = ASC->GetSet<UURRUnitAttributeSet>();
	if (!AttrubuteSet) return FGameplayAbilityTargetDataHandle();

	TArray<FHitResult> OutHitResults;
	const float AttackRange = AttrubuteSet->GetAttackRange();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UURRAttack), false, Character);
	const FVector Center = Character->GetActorLocation();

	bool bHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Center, Center, FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeSphere(AttackRange), Params);

	TArray<TWeakObjectPtr<AActor>> HittedActors;
	if (bHitDetected)
	{
		for (auto hitResult : OutHitResults)
		{
			AURRCharacterMonster* Monster = CastChecked<AURRCharacterMonster>(hitResult.GetActor());
			HittedActors.Add(Monster);
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
		FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), CapsuleOrigin, AttackRange, 32, DrawColor, false, 5.f);
	}

#endif

	return DataHandle;
}
