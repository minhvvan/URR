// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRMonsterAttributeSet.h"
#include "URR.h"

void UURRHpBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UURRMonsterAttributeSet::GetHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UURRMonsterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UURRHpBarWidget::OnMaxHealthChanged);

		const UURRMonsterAttributeSet* CurrentAttributeSet = ASC->GetSet<UURRMonsterAttributeSet>();
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