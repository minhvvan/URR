// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "URRUnitAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class URR_API UURRUnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UURRUnitAttributeSet();

	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, Rank);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, CriticalProbability);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, CriticalAttackRate);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, KnockBackDist);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, SlowRate);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, FindTargetType);
	ATTRIBUTE_ACCESSORS(UURRUnitAttributeSet, AdditiveAttackRate);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Rank;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalProbability;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData KnockBackDist;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SlowRate;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FindTargetType;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AdditiveAttackRate;

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
