// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/URRExecuteGAExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "URR.h"

void UURRExecuteGAExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	if (!SourceASC) return;

	auto Spec = ExecutionParams.GetOwningSpec();
	for (auto& GASpec : Spec.Def->GrantedAbilities)
	{
		SourceASC->TryActivateAbility(GASpec.AssignedHandle);
	}
}