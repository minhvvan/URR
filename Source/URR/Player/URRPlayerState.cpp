// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/URRPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PlayerAttributeSet.h"

AURRPlayerState::AURRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

UAbilitySystemComponent* AURRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
