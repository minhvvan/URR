// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRTile.h"

// Sets default values
AURRTile::AURRTile()
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

// Called every frame
void AURRTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

