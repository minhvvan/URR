// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/URRGASWidget.h"
#include "GameplayEffectTypes.h"
#include "URRHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRHpBarWidget : public UURRGASWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PBHealth;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

protected:
	float CurrentHealth;
	float CurrentMaxHealth;
};
