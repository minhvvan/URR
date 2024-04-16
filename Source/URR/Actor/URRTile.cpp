// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRTile.h"
#include "Character/URRCharacterUnit.h"
#include "Engine/AssetManager.h"
#include "Urr.h"

// Sets default values
AURRTile::AURRTile(): isEmpty(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	SetRootComponent(TileMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/URR/Mesh/TileMesh.TileMesh'"));
	if (TileMeshRef.Succeeded())
	{
		TileMesh->SetStaticMesh(TileMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AURRTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AURRTile::UnitLoadCompleteCallback()
{
	FVector SpawnLoc = GetActorLocation();
	SpawnLoc.Z += 138;
	FTransform FinalTransform = FTransform(FRotator::ZeroRotator, SpawnLoc);
	UnitCharacter->FinishSpawning(FinalTransform);
}

// Called every frame
void AURRTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AURRTile::IsEmpty()
{
	return isEmpty;
}

void AURRTile::SpawnUnit(int rank)
{
	if (rank > 10)
	{
		//Shuffle
		URR_LOG(LogURR, Log, TEXT("Shffle"));
		return;
	}

	Rank = rank;

	int32 Idx;
	if (rank < 4) Idx = 0;
	else if (rank == 4) Idx = 1;
	else if (rank == 5) Idx = 2;
	else if (rank == 6) Idx = 3;
	else Idx = 4;

	UnitClass = UnitClasses[Idx].TryLoadClass<AURRCharacterUnit>();
	if (UnitClass)
	{
		//Spawn Uit
		FVector SpawnLoc = GetActorLocation();
		SpawnLoc.Z += 138;

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//UnitCharacter = GetWorld()->SpawnActor<AURRCharacterUnit>(UnitClass, SpawnLoc, FRotator::ZeroRotator, params);
		UnitCharacter = GetWorld()->SpawnActorDeferred<AURRCharacterUnit>(UnitClass, FTransform::Identity, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		UnitCharacter->OnLoadCompleteDelegate.AddDynamic(this, &AURRTile::UnitLoadCompleteCallback);
		UnitCharacter->Init(Rank);

		isEmpty = false;
	}
}

void AURRTile::RankUpUnit()
{
	int CurrentRank = Rank;
	DestroyUnit();

	SpawnUnit(CurrentRank + 1);
}

void AURRTile::DestroyUnit()
{
	if (UnitCharacter)
	{
		UnitCharacter->Destroy();
		UnitCharacter = nullptr;
		isEmpty = true;
		Rank = 0;
	}
}

int AURRTile::GetRank()
{
	return Rank;
}
