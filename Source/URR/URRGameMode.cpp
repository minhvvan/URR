// Copyright Epic Games, Inc. All Rights Reserved.

#include "URRGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/URRPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/URRGameInstance.h"
#include "Framework/URRPlayerController.h"
#include "URR.h"

AURRGameMode::AURRGameMode():
	LobbyLevelName(TEXT("Lobby"))
{
	// use our custom PlayerController class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Board.BP_Board_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AURRPlayerController::StaticClass();
	PlayerStateClass = AURRPlayerState::StaticClass();
}

void AURRGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AURRGameMode::OpenLobbyLevel()
{
	//TODO: stage clear data : saveGame
	UGameplayStatics::OpenLevel(this, FName(LobbyLevelName));
}