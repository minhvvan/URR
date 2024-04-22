// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/URRPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"

AURRPlayerState::AURRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	URRPlayerAttributeSet = CreateDefaultSubobject<UURRPlayerAttributeSet>(TEXT("URRPlayerAttributeSet"));
}

UAbilitySystemComponent* AURRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
