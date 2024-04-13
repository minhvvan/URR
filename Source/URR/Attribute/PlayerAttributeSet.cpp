// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PlayerAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/URRGameplayTag.h"

UPlayerAttributeSet::UPlayerAttributeSet() :
	Coin(1000),
	Health(100),
	MaxHealth(100)
{
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
		float cost = GetCoin() - GetCost();
		if (cost < 0) cost = 0.f;

		SetCoin(cost);
		SetCost(0.f);

		if (GetCoin() < 10)
		{
			Data.Target.RemoveLooseGameplayTag(URRTAG_PLAYER_CANSPAWN);
		}
	}
}