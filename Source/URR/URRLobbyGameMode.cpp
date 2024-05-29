// Fill out your copyright notice in the Description page of Project Settings.


#include "URRLobbyGameMode.h"
#include "Framework/URRLobbyPC.h"

AURRLobbyGameMode::AURRLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_LobbyCharacter.BP_LobbyCharacter_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AURRLobbyPC::StaticClass();
}