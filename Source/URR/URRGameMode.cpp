// Copyright Epic Games, Inc. All Rights Reserved.

#include "URRGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/URRPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/URRMonsterSpawner.h"
#include "Framework/URRGameInstance.h"
#include "URR.h"

AURRGameMode::AURRGameMode()
{
	// use our custom PlayerController class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Board.BP_Board_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerStateClass = AURRPlayerState::StaticClass();
}

void AURRGameMode::BeginPlay()
{
	Super::BeginPlay();
	AURRMonsterSpawner* Spawner = Cast<AURRMonsterSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AURRMonsterSpawner::StaticClass()));

	UURRGameInstance* GameInstance = GetGameInstance<UURRGameInstance>();
	TArray<FMonsterInfo> Waves = GameInstance->GetWaveInfo(1);

	if (Spawner && !Waves.IsEmpty())
	{
		Spawner->SetWaveInfo(Waves);
		Spawner->SpawnMonster();
	}
}
