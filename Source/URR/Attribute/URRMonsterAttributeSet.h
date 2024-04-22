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
};
