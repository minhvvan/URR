// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URRGASWidgetComponent.h"
#include "UI/URRGASWidget.h"

void UURRGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UURRGASWidget* GASUserWidget = Cast<UURRGASWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}