// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "URRCharacterUnit.generated.h"

/**
 * 
 */
UCLASS(config = URR)
class URR_API AURRCharacterUnit : public AURRCharacterBase
{
	GENERATED_BODY()
	
public:
	AURRCharacterUnit();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<class UURRUnitAttributeSet> UnitAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

protected:
	int Rank;

public:
	virtual void Init(int rank);

	void AttackMontageLoadCompleted();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AttackMontages;
	TSharedPtr<FStreamableHandle> AttackMontageHandle;

	UPROPERTY(config)
	TArray<FName> SocketName;
};
