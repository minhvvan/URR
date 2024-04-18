// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PlayerAttributeSet.h"

void UURRHpBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnMaxHealthChanged);

		const UPlayerAttributeSet* CurrentAttributeSet = ASC->GetSet<UPlayerAttributeSet>();
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