// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRHudWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Character/URRBoard.h"
#include "Character/URRCharacterUnit.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "Animation/WidgetAnimation.h"
#include "Framework/URRGameInstance.h"
#include "Internationalization/StringTable.h"
#include "URR.h"

UURRHudWidget::UURRHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> DTRef(TEXT("/Script/Engine.DataTable'/Game/URR/Data/DT_FindTargetText.DT_FindTargetText'"));
	if (DTRef.Succeeded())
	{
		FindTargetTable = DTRef.Object;
	}
}

void UURRHudWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	Board = Cast<AURRBoard>(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetCoinAttribute()).AddUObject(this, &UURRHudWidget::OnCoinChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &UURRHudWidget::OnHealthChanged);

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

void UURRHudWidget::ShowUnitInfo(AURRCharacterUnit* SelectedUnit)
{
	UAbilitySystemComponent* UnitASC = SelectedUnit->GetAbilitySystemComponent();
	if (!UnitASC) return;

	const UURRUnitAttributeSet* UnitAttribute = UnitASC->GetSet<UURRUnitAttributeSet>();
	if (!UnitAttribute) return;

	BorderUnitInfo->SetVisibility(ESlateVisibility::Visible);
	TxtUnitRank->SetText(FText::AsNumber(UnitAttribute->GetRank()));
	TxtUnitAttackRate->SetText(FText::AsNumber(UnitAttribute->GetAttackRate()));
	TxtUnitAttackSpeed->SetText(FText::AsNumber(UnitAttribute->GetAttackSpeed()));
	TxtUnitCritPob->SetText(FText::AsNumber(UnitAttribute->GetCriticalProbability()));
	TxtUnitCritMult->SetText(FText::AsNumber(UnitAttribute->GetCriticalAttackRate()));
	TxtUnitTarget->SetText(GetFindTargetTypeText(UnitAttribute->GetFindTargetType()));

	BorderUnitInfo->SetVisibility(ESlateVisibility::Visible);
}

void UURRHudWidget::HideUnitInfo()
{
	BorderUnitInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UURRHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
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

FText UURRHudWidget::GetFindTargetTypeText(float type)
{
	if (!FindTargetTable) return FText();

	int temp = type;
	return FText::FromString(FindTargetTable->FindRow<FFindTargetText>(FName(FString::Printf(TEXT("%d"), temp)), TEXT(""))->Text);
}
