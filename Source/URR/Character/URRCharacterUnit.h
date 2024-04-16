// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "URRCharacterUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadCompleted);
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
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY()
	TObjectPtr<class UURRUnitAttributeSet> UnitAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Init(int rank);

	void AttackMontageLoadCompleted();

	FORCEINLINE class UAnimMontage* GetPlacedActionMontage() const { return PlacedActionMontage; }

	FOnLoadCompleted OnLoadCompleteDelegate;

protected:
	int Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> PlacedActionMontage;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AttackMontages;
	TSharedPtr<FStreamableHandle> AttackMontageHandle;

	UPROPERTY(config)
	TArray<FName> SocketName;
};
