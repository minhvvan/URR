// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "URRCharacterMonster.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRCharacterMonster : public AURRCharacterBase
{
	GENERATED_BODY()
	
public:
	AURRCharacterMonster();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY()
	TObjectPtr<class UMonsterAttributeSet> MonsterAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMeshes;
	TSharedPtr<FStreamableHandle> MonsterMeshHandle;
};