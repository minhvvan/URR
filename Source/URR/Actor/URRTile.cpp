// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRTile.h"
#include "Character/URRCharacterUnit.h"
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

	ConstructorHelpers::FClassFinder<AURRCharacterUnit> UnitRef(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Unit.BP_Unit_C'"));
	if (UnitRef.Succeeded())
	{
		UnitClass = UnitRef.Class;
	}
}

// Called when the game starts or when spawned
void AURRTile::BeginPlay()
{
	Super::BeginPlay();
	
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
	//Spawn Uit
	FVector SpawnLoc = GetActorLocation();
	SpawnLoc.Z += 50;

	FActorSpawnParameters params;

	UnitCharacter = GetWorld()->SpawnActor<AURRCharacterUnit>(UnitClass, SpawnLoc, FRotator::ZeroRotator, params);
	UnitCharacter->Init(rank);

	isEmpty = false;
}