// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/URRPlayerAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/URRGameplayTag.h"

UURRPlayerAttributeSet::UURRPlayerAttributeSet() :
	Coin(1000),
	Health(2048),
	MaxHealth(2048),
	RewardLevel(1),
	Cost(0),
	Damage(0),
	BonusCoin(0),
	PrevHealth(0)
{
}

void UURRPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetCoinAttribute())
	{
		int coin = GetCoin();
		if (coin >= 10)
		{
			if (Data.Target.GetTagCount(URRTAG_PLAYER_CANSPAWN) == 0)
			{
				Data.Target.AddLooseGameplayTag(URRTAG_PLAYER_CANSPAWN);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetCostAttribute())
	{
		float newCoin = GetCoin() - GetCost() + GetBonusCoin();
		if (newCoin < 0) newCoin = 0.f;

		SetCoin(newCoin);
		SetCost(0.f);

		if (GetCoin() < 10)
		{
			Data.Target.RemoveLooseGameplayTag(URRTAG_PLAYER_CANSPAWN);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float MinHealth = 0.f;

		SetPrevHealth(GetHealth());

		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.f);

		if (GetHealth() < 0.f)
		{
			Data.Target.RemoveLooseGameplayTag(URRTAG_PLAYER_CANSPAWN);
		}
	}
}