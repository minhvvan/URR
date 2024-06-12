// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/URRAttackDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "Attribute/URRMonsterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "URR.h"

void UURRAttackDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			const UURRUnitAttributeSet* UnitAttribute = SourceASC->GetSet<UURRUnitAttributeSet>();
			if (UnitAttribute)
			{
				float Damage = UnitAttribute->GetAttackRate();

				int rand = FMath::Rand() % 100;
				float Prob = UnitAttribute->GetCriticalProbability();
				if(rand <= Prob) Damage = UnitAttribute->GetCriticalAttackRate();

				OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UURRMonsterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
			}
		}
	}
}