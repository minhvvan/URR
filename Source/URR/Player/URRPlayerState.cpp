// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/URRPlayerState.h"
#include "AbilitySystemComponent.h"

AURRPlayerState::AURRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AURRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
