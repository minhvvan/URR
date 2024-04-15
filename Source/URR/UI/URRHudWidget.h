// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "GameplayEffectTypes.h"
#include "URRHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRHudWidget : public UURRGASWidget
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UButton* BtnSpawn;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtCoin;	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UProgressBar* PBHealth;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtHealth;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtMaxHealth;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnSpawnClicked();

	virtual void OnCoinChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	void UpdateCoin();

	void UpdateHealth();

protected:
	TObjectPtr<class AURRBoard> Board;

	int CurrentCoin;

	int CurrentHealth;
	int CurrentMaxHealth;
};
