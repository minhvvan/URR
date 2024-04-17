// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MonsterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class URR_API UMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UMonsterAttributeSet();

	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, ID);
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UMonsterAttributeSet, Speed);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ID;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Speed;
};
