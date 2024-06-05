// Fill out your copyright notice in the Description page of Project Settings.


#include "URRLobbyGameMode.h"
#include "Framework/URRLobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/URRGameInstance.h"

AURRLobbyGameMode::AURRLobbyGameMode(): 
	StageLevelName(TEXT("Stage"))
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_LobbyCharacter.BP_LobbyCharacter_C'"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AURRLobbyPC::StaticClass();
}

void AURRLobbyGameMode::OpenStageLevel(int stageNum)
{
	UURRGameInstance* GI = Cast<UURRGameInstance>(GetGameInstance());
	if (!GI) return;

	GI->SetStageNum(stageNum);
	UGameplayStatics::OpenLevel(this, FName(StageLevelName));
}