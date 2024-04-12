

#include "Character/URRBoard.h"
#include "Actor/URRTile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "UI/URRHudWidget.h"
#include "Player/URRPlayerState.h"
#include "GA/URRGA_SpawnUnit.h"
#include "Urr.h"


AURRBoard::AURRBoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoardArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoardArea"));
	SetRootComponent(BoardArea);
	BoardArea->SetBoxExtent(FVector(1500, 1500, 1000));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2400.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CastleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastleMesh"));
	CastleMesh->SetupAttachment(RootComponent);	
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UURRHudWidget> HUDRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/URR/UI/WBP_Hud.WBP_Hud_C'"));
	if (HUDRef.Succeeded())
	{
		HudClass = HUDRef.Class;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> CastleMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/URR/Mesh/WallMesh.WallMesh'"));
	if (CastleMeshRef.Succeeded())
	{
		CastleMesh->SetStaticMesh(CastleMeshRef.Object);
		CastleMesh->SetRelativeLocation(FVector(1000, -100, -400));
	}
}

UAbilitySystemComponent* AURRBoard::GetAbilitySystemComponent() const
{
	return ASC;
}

void AURRBoard::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AURRPlayerState* PS = Cast<AURRPlayerState>(GetPlayerState());
	if (PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(PS, this);

		for (auto StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
	}

	APlayerController* PC = CastChecked<APlayerController>(GetController());
	PC->ConsoleCommand(TEXT("showdebug abilitysystem"));
}

// Called when the game starts or when spawned
void AURRBoard::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeGameAndUI());

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
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

	if (HudClass)
	{
		HudWidget = CreateWidget<UURRHudWidget>(GetWorld(), HudClass);
		if (HudWidget) HudWidget->AddToViewport();
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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveInputPressed, 0);
	EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveInputPressed, 1);
	EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveInputPressed, 2);
	EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveInputPressed, 3);
	EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &AURRBoard::Test, 0);
}

void AURRBoard::MoveInputPressed(int32 InputId)
{
	//Move Tiles
	TArray<int> dy = { 0, 0, -1, 1 };
	TArray<int> dx = { -1, 1, 0, 0 };

}

void AURRBoard::Test(int32 InputId)
{
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Testffect, 0, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AURRBoard::SpawnUnit()
{
	//GA Activate
	if (!ASC) return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(SpawnAbilityClass);
	if (Spec)
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

AURRTile* AURRBoard::GetEmptyTile()
{
	TArray<AURRTile*> EmptyTiles;
	for (auto Tile : Tiles)
	{
		for (auto t : Tile)
		{
			if (t->IsEmpty()) EmptyTiles.Add(t);
		}
	}

	if (EmptyTiles.Num() == 0) return nullptr;

	int rand = FMath::Rand() % EmptyTiles.Num();
	return EmptyTiles[rand];
}
