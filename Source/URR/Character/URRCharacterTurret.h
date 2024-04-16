// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterUnit.h"
#include "URRCharacterTurret.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRCharacterTurret : public AURRCharacterUnit
{
	GENERATED_BODY()
	
public:
	AURRCharacterTurret();

	virtual void Init(int rank) override;
};