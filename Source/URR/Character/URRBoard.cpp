

#include "Character/URRBoard.h"
#include "Actor/URRTile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"

AURRBoard::AURRBoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoardArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoardArea"));
	SetRootComponent(BoardArea);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2400.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AURRBoard::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	FRotator Rotator;
	FVector SpawnLocation  = FVector::ZeroVector;
	SpawnLocation.Z = 600;

	Tiles.SetNum(4);

	for (int i = 0; i < 4; i++)
	{
		SpawnLocation.X = 530 * i;
		SpawnLocation.Y = 0;

		for (int j = 0; j < 4; j++)
		{
			AURRTile* Tile = GetWorld()->SpawnActor<AURRTile>(AURRTile::StaticClass(), SpawnLocation, Rotator, SpawnParams);
			Tiles[i].Add(Tile);

			SpawnLocation.Y += 530;
		}
	}
}

// Called every frame
void AURRBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AURRBoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

