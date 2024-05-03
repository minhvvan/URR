// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "URRGameMode.generated.h"

UCLASS(minimalapi)
class AURRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AURRGameMode();

protected:
	virtual void BeginPlay() override;
};