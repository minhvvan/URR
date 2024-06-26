// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/URRMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"

UURRMonsterAttributeSet::UURRMonsterAttributeSet():
	ID(0),
	Health(1),
	MaxHealth(1),
	Speed(0),
	Distance(0),
	KnockBackDistance(0),
	Reward(1)
{
}

void UURRMonsterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

bool UURRMonsterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UURRMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	float MinimumHealth = 0.f;
	float MinimumDistance = 0.f;

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.f);
	}
	
	if (Data.EvaluatedData.Attribute == GetKnockBackDistanceAttribute())
	{
		SetDistance(FMath::Clamp(GetDistance() - GetKnockBackDistance(), MinimumDistance, GetDistance()));
		SetKnockBackDistance(0.f);
	}

	if ((GetHealth() <= 0.f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(URRTAG_MONSTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.f);
}