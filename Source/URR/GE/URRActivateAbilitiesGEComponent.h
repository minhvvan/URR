// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "URRActivateAbilitiesGEComponent.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRActivateAbilitiesGEComponent : public UAbilitiesGameplayEffectComponent
{
	GENERATED_BODY()
	
protected:
	virtual void GrantAbilities(FActiveGameplayEffectHandle ActiveGEHandle) const;

};
