// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "URRMonsterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

UCLASS()
class URR_API UURRMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UURRMonsterAttributeSet();

	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, ID);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, Speed);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, Distance);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, KnockBackDistance);
	ATTRIBUTE_ACCESSORS(UURRMonsterAttributeSet, Reward);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data);
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ID;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Speed;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Distance;	
	
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData KnockBackDistance;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Reward;

	bool bOutOfHealth = false;
};
