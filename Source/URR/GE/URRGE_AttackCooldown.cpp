// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/URRGE_AttackCooldown.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRUnitAttributeSet.h"

UURRGE_AttackCooldown::UURRGE_AttackCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}
