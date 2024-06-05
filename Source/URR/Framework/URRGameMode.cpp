// Copyright Epic Games, Inc. All Rights Reserved.

#include "URRGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/URRPlayerState.h"
#include "Character/URRBoard.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/URRGameInstance.h"
#include "Framework/URRPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"
#include "Framework/URRStageSG.h"
#include "Framework/URRGameInstance.h"
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

	AURRBoard* Board = Cast<AURRBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), AURRBoard::StaticClass()));
	if (!Board) return;

	UAbilitySystemComponent* ASC = Board->GetAbilitySystemComponent();
	if (!ASC) return;
	
	const UURRPlayerAttributeSet* AttributeSet = ASC->GetSet<UURRPlayerAttributeSet>();
	if (!AttributeSet) return;

	const int MAXHEALTH = 2048;
	float Health = AttributeSet->GetHealth();

	int score = (Health / MAXHEALTH) * 100;
	
	UURRGameInstance* GI = GetGameInstance<UURRGameInstance>();
	if (!GI) return;

	int stage = GI->GetStageNum();

	UURRStageSG* SaveGameInstance = Cast<UURRStageSG>(UGameplayStatics::CreateSaveGameObject(UURRStageSG::StaticClass()));
	SaveGameInstance->Score = score;
	SaveGameInstance->StageLevel = stage;

	FString SlotName(TEXT("Stage"));
	SlotName += FString::Printf(TEXT("%d"), stage);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, SaveGameInstance->UserIndex);
	UGameplayStatics::OpenLevel(this, FName(LobbyLevelName));
}