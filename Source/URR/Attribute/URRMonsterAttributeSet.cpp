// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/URRMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "URR.h"

UURRMonsterAttributeSet::UURRMonsterAttributeSet():
	ID(0),
	Health(0),
	MaxHealth(0),
	Speed(0),
	Distance(0)
{
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

	URR_LOG(LogURR, Log, TEXT("PostGameplayEffectExecute"));

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.f);
	}

	if ((GetHealth() <= 0.f) && !bOutOfHealth)
	{
		//Data.Target.AddLooseGameplayTag(ABTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.f);
}
