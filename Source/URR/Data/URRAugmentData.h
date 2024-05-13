// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayEffect.h"
#include "URRAugmentData.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRAugmentData : public UObject
{
	GENERATED_BODY()
	
public:
	FString Title;
	FString Desc;
	UTexture2D* Icon;
	TSubclassOf<UGameplayEffect> GE;
	TArray<int> Targets;
};
