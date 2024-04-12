// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "URRCharacterUnit.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRCharacterUnit : public AURRCharacterBase
{
	GENERATED_BODY()
	
public:
	AURRCharacterUnit();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay();

	UPROPERTY()
	TObjectPtr<class UURRUnitAttributeSet> UnitAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;
public:
	void Init(int rank);
};
