// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class URR_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Coin);
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxHealth);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Coin;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
};
