// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"

void UURRHpBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnMaxHealthChanged);

		const UURRPlayerAttributeSet* CurrentAttributeSet = ASC->GetSet<UURRPlayerAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			UpdateHpBar();
		}
	}
}

void UURRHpBarWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UURRHpBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UURRHpBarWidget::UpdateHpBar()
{
	PBHealth->SetPercent(CurrentHealth / CurrentMaxHealth);
}