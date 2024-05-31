// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "URRLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AURRLobbyGameMode();

	void OpenStageLevel(int stageNum);

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString StageLevelName;
};
