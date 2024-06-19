// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "URRPlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class URR_API UURRPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UURRPlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, Coin);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, RewardLevel);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, Cost);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, BonusCoin);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, PrevHealth);
	ATTRIBUTE_ACCESSORS(UURRPlayerAttributeSet, Shield);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Coin;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RewardLevel;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Cost;	
	
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BonusCoin;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PrevHealth;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};