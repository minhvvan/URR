// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "URRProjectileAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class URR_API UURRProjectileAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UURRProjectileAttributeSet();

	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, KnockBackDistance);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, SlowRate);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, ExplosionRange);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, CriticalProb);
	ATTRIBUTE_ACCESSORS(UURRProjectileAttributeSet, CriticalRate);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData KnockBackDistance;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SlowRate;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ExplosionRange;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalProb;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalRate;
};
