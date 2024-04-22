// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHudWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/URRBoard.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"

void UURRHudWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	Board = Cast<AURRBoard>(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetCoinAttribute()).AddUObject(this, &UURRHudWidget::OnCoinChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UURRHudWidget::OnCoinChanged);
		//ASC->RegisterGameplayTagEvent(ABTAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UABGASHpBarUserWidget::OnInvinsibleTagChanged);

		const UURRPlayerAttributeSet* CurrentAttributeSet = ASC->GetSet<UURRPlayerAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentCoin = CurrentAttributeSet->GetCoin();
			UpdateCoin();

			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			TxtMaxHealth->SetText(FText::AsNumber(CurrentMaxHealth));
			UpdateHealth();
		}
	}
}

void UURRHudWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BtnSpawn->OnClicked.AddDynamic(this, &UURRHudWidget::OnSpawnClicked);
}

void UURRHudWidget::OnSpawnClicked()
{
	if (Board.Get() == nullptr)
	{
		Board = CastChecked<AURRBoard>(GetOwningPlayerPawn());
	}

	Board->SpawnUnit();
}

void UURRHudWidget::OnCoinChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentCoin = ChangeData.NewValue;
	UpdateCoin();
}

void UURRHudWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UURRHudWidget::UpdateCoin()
{
	TxtCoin->SetText(FText::AsNumber(CurrentCoin));
}

void UURRHudWidget::UpdateHealth()
{
	TxtHealth->SetText(FText::AsNumber(CurrentHealth));
	PBHealth->SetPercent(CurrentHealth / CurrentMaxHealth);
}