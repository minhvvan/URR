// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRGASWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

UAbilitySystemComponent* UURRGASWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

void UURRGASWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}