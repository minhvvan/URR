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
	UURRHudWidget(const FObjectInitializer& ObjectInitializer);

	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	void ShowUnitInfo(class AURRCharacterUnit* SelectedUnit);
	void HideUnitInfo();

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

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UBorder* BorderUnitInfo;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitRank;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitAttackRate;	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitAttackSpeed;	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitCritPob;	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitCritMult;	

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* TxtUnitTarget;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetAnimation> WaveNotifyAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDataTable> FindTargetTable;

protected:
	virtual void NativeConstruct();
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnSpawnClicked();

	virtual void OnCoinChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	void UpdateCoin();

	void UpdateHealth();

	FText GetFindTargetTypeText(float type);

protected:
	TObjectPtr<class AURRBoard> Board;

	int CurrentCoin;

	float CurrentHealth;
	float CurrentMaxHealth;
};
