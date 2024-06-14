// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/URRUnitAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/URRGameplayTag.h"

UURRUnitAttributeSet::UURRUnitAttributeSet() :
	Rank(0),
	AttackRate(1),
	AttackRange(100),
	AttackSpeed(1),
	CriticalProbability(1),
	CriticalAttackRate(2),
	AdditiveAttackRate(0)
{

}

void UURRUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}