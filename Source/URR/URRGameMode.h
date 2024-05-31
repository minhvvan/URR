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

	void OpenLobbyLevel();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString LobbyLevelName;
};