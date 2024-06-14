

#include "Character/URRBoard.h"
#include "Actor/URRTile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRPlayerAttributeSet.h"
#include "UI/URRHudWidget.h"
#include "Player/URRPlayerState.h"
#include "GA/URRGA_SpawnUnit.h"
#include "Tag/URRGameplayTag.h"
#include "Framework/URRPlayerController.h"
#include "Framework/URRWaveManager.h"
#include "Character/URRCharacterUnit.h"
#include "Physics/URRCollision.h"
#include "Urr.h"

enum MoveDir
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

AURRBoard::AURRBoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoardArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoardArea"));
	SetRootComponent(BoardArea);
	//BoardArea->SetBoxExtent(FVector(1500, 1500, 1000));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2400.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CastleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastleMesh"));
	CastleMesh->SetupAttachment(RootComponent);	
	
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
	
	ConstructorHelpers::FClassFinder<AURRTile> TileClassRef(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Tile.BP_Tile_C'"));
	if (TileClassRef.Succeeded())
	{
		TileClass = TileClassRef.Class;
	}

	ConstructorHelpers::FClassFinder<UGameplayEffect> GetCoinGERef(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/GE/BPGE_GetCoin.BPGE_GetCoin_C'"));
	if (GetCoinGERef.Succeeded())
	{
		GetCoinEffect = GetCoinGERef.Class;
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
			FGameplayAbilitySpec StartSpec(StartAbility.Value);
			StartSpec.InputID = StartAbility.Key;

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
	SpawnParams.Owner = this;
	FRotator Rotator;
	FVector SpawnLocation  = FVector::ZeroVector;
	SpawnLocation.Z = 600;

	Tiles.SetNum(4);

	for (int i = 0; i < 4; i++)
	{
		SpawnLocation.X = 1590 - (530 * i);
		SpawnLocation.Y = 0;

		for (int j = 0; j < 4; j++)
		{
			AURRTile* Tile = GetWorld()->SpawnActor<AURRTile>(TileClass, SpawnLocation, Rotator, SpawnParams);
			Tiles[i].Add(Tile);

			SpawnLocation.Y += 530;
		}
	}

	if (HudClass)
	{
		HudWidget = CreateWidget<UURRHudWidget>(GetWorld(), HudClass);
		if (HudWidget)
		{
			HudWidget->AddToViewport();
			HudWidget->SetAbilitySystemComponent(this);
		}
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GetCoinEffect, 0, EffectContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(URRTAG_PLAYER_GETCOIN, 10.f);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	
	ASC->GetGameplayAttributeValueChangeDelegate(UURRPlayerAttributeSet::GetHealthAttribute()).AddUObject(this, &AURRBoard::OnHealthChanged);
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
	
	EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveLeft);
	EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveRight);
	EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveUp);
	EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &AURRBoard::MoveDown);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AURRBoard::OnClicked);
}


void AURRBoard::OnClicked()
{
	AURRPlayerController* PC = Cast<AURRPlayerController>(GetController());
	if (!PC) return;

	FHitResult result;
	if (PC->GetHitResultUnderCursor(CCHANNEL_URRCLICK, true, result))
	{
		if(SelectedUnit) SelectedUnit->SetShowRangeIndicator(false);
		SelectedUnit = Cast<AURRCharacterUnit>(result.GetActor());
		if (!SelectedUnit) return;

		SelectedUnit->SetShowRangeIndicator(true);
		HudWidget->ShowUnitInfo(SelectedUnit);
	}
	else
	{
		if (!SelectedUnit) return;
		SelectedUnit->SetShowRangeIndicator(false);

		SelectedUnit = nullptr;
		HudWidget->HideUnitInfo();
	}
}


bool AURRBoard::IsValidIdx(int y, int x)
{
	return y >= 0 && y < 4 && x >= 0 && x < 4;
}

void AURRBoard::MoveLeft()
{
	if (ASC->HasMatchingGameplayTag(URRTAG_PLAYER_ISSPAWNING)) return;

	for (int i = 0; i < 4; i++)
	{
		TArray<int> ExistSet;
		TArray<int> EmptySet;

		for (int j = 0; j < 4; j++)
		{
			if (Tiles[i][j]->IsEmpty()) EmptySet.Add(j);
			else ExistSet.Add(j);
		}

		ExistSet.Heapify();
		EmptySet.Heapify();

		while(!ExistSet.IsEmpty())
		{
			int currentIdx;
			ExistSet.HeapPop(currentIdx, true);
			int currentRank = Tiles[i][currentIdx]->GetRank();

			int prevRank = -1;
			int prevIdx = -1;

			for (int j = currentIdx - 1; j >= 0; j--)
			{
				if (!Tiles[i][j]->IsEmpty())
				{
					prevRank = Tiles[i][j]->GetRank();
					prevIdx = j;
					break;
				}
			}

			if (currentRank == prevRank)
			{
				Tiles[i][currentIdx]->DestroyUnit();
				Tiles[i][prevIdx]->RankUpUnit();

				ExistSet.HeapPush(prevIdx);
			}
			else
			{
				if (EmptySet.IsEmpty()) continue;
				if (currentIdx < EmptySet.HeapTop()) continue;

				int firstEmpty;
				EmptySet.HeapPop(firstEmpty, true);

				Swap(Tiles[i][firstEmpty]->UnitCharacter, Tiles[i][currentIdx]->UnitCharacter);
				Tiles[i][firstEmpty]->AdjustUnit();
				Tiles[i][currentIdx]->AdjustUnit();
			}
		
			EmptySet.HeapPush(currentIdx);
		}
	}
}

void AURRBoard::MoveRight()
{
	if (ASC->HasMatchingGameplayTag(URRTAG_PLAYER_ISSPAWNING)) return;
	for (int i = 0; i < 4; i++)
	{
		TArray<int> ExistSet;
		TArray<int> EmptySet;

		for (int j = 3; j >= 0; j--)
		{
			if (Tiles[i][j]->IsEmpty())
			{
				EmptySet.Add(j);
			}
			else ExistSet.Add(j);
		}

		ExistSet.Heapify(TGreater<int>());
		EmptySet.Heapify(TGreater<int>());

		while (!ExistSet.IsEmpty())
		{
			int currentIdx;
			ExistSet.HeapPop(currentIdx, TGreater<int>(), true);
			int currentRank = Tiles[i][currentIdx]->GetRank();

			int prevRank = -1;
			int prevIdx = -1;

			for (int j = currentIdx+1; j < 4; j++)
			{
				if (!Tiles[i][j]->IsEmpty())
				{
					prevRank = Tiles[i][j]->GetRank();
					prevIdx = j;
					break;
				}
			}

			if (currentRank == prevRank)
			{
				Tiles[i][currentIdx]->DestroyUnit();
				Tiles[i][prevIdx]->RankUpUnit();

				ExistSet.HeapPush(prevIdx, TGreater<int>());
			}
			else
			{
				if (EmptySet.IsEmpty()) continue;
				if (currentIdx > EmptySet.HeapTop()) continue;

				int firstEmpty;
				EmptySet.HeapPop(firstEmpty, TGreater<int>(), true);

				Swap(Tiles[i][firstEmpty]->UnitCharacter, Tiles[i][currentIdx]->UnitCharacter);
				Tiles[i][firstEmpty]->AdjustUnit();
				Tiles[i][currentIdx]->AdjustUnit();
			}

			EmptySet.HeapPush(currentIdx, TGreater<int>());
		}
	}
}

void AURRBoard::MoveUp()
{
	if (ASC->HasMatchingGameplayTag(URRTAG_PLAYER_ISSPAWNING)) return;
	for (int j = 0; j < 4; j++)
	{
		TArray<int> ExistSet;
		TArray<int> EmptySet;

		for (int i = 0; i < 4; i++)
		{
			if (Tiles[i][j]->IsEmpty())
			{
				EmptySet.Add(i);
			}
			else ExistSet.Add(i);
		}

		ExistSet.Heapify();
		EmptySet.Heapify();

		while (!ExistSet.IsEmpty())
		{
			int currentIdx;
			ExistSet.HeapPop(currentIdx, true);
			int currentRank = Tiles[currentIdx][j]->GetRank();

			int prevRank = -1;
			int prevIdx = -1;

			for (int i = currentIdx - 1; i >= 0; i--)
			{
				if (!Tiles[i][j]->IsEmpty())
				{
					prevRank = Tiles[i][j]->GetRank();
					prevIdx = i;
					break;
				}
			}

			if (currentRank == prevRank)
			{
				Tiles[currentIdx][j]->DestroyUnit();
				Tiles[prevIdx][j]->RankUpUnit();

				ExistSet.HeapPush(prevIdx);
			}
			else
			{
				if (EmptySet.IsEmpty()) continue;
				if (currentIdx < EmptySet.HeapTop()) continue;

				int firstEmpty;
				EmptySet.HeapPop(firstEmpty, true);

				Swap(Tiles[currentIdx][j]->UnitCharacter, Tiles[firstEmpty][j]->UnitCharacter);
				Tiles[currentIdx][j]->AdjustUnit();
				Tiles[firstEmpty][j]->AdjustUnit();
			}
			
			EmptySet.HeapPush(currentIdx);
		}
	}
}

void AURRBoard::MoveDown()
{
	if (ASC->HasMatchingGameplayTag(URRTAG_PLAYER_ISSPAWNING)) return;
	for (int j = 0; j < 4; j++)
	{
		TArray<int> ExistSet;
		TArray<int> EmptySet;

		for (int i = 3; i >= 0; i--)
		{
			if (Tiles[i][j]->IsEmpty())
			{
				EmptySet.Add(i);
			}
			else ExistSet.Add(i);
		}

		ExistSet.Heapify(TGreater<int>());
		EmptySet.Heapify(TGreater<int>());

		while (!ExistSet.IsEmpty())
		{
			int currentIdx;
			ExistSet.HeapPop(currentIdx, TGreater<int>(), true);
			int currentRank = Tiles[currentIdx][j]->GetRank();

			int prevRank = -1;
			int prevIdx = -1;

			for (int i = currentIdx + 1; i < 4; i++)
			{
				if (!Tiles[i][j]->IsEmpty())
				{
					prevRank = Tiles[i][j]->GetRank();
					prevIdx = i;
					break;
				}
			}

			if (currentRank == prevRank)
			{
				Tiles[currentIdx][j]->DestroyUnit();
				Tiles[prevIdx][j]->RankUpUnit();

				ExistSet.HeapPush(prevIdx, TGreater<int>());
			}
			else
			{
				if (EmptySet.IsEmpty()) continue;
				if (currentIdx > EmptySet.HeapTop()) continue;

				int firstEmpty;
				EmptySet.HeapPop(firstEmpty, TGreater<int>(), true);

				Swap(Tiles[currentIdx][j]->UnitCharacter, Tiles[firstEmpty][j]->UnitCharacter);
				Tiles[currentIdx][j]->AdjustUnit();
				Tiles[firstEmpty][j]->AdjustUnit();
			}
			
			EmptySet.HeapPush(currentIdx, TGreater<int>());
		}
	}
}

void AURRBoard::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	UURRWaveManager* WaveManager = Cast<UURRWaveManager>(GEngine->GameSingleton);
	if (!WaveManager) return;

	for (int rank = 0; rank < 11; rank++)
	{
		TArray<FAugment*> Augments = WaveManager->GetAugments(rank);

		for (auto augment : Augments)
		{
			if (!augment->bRelateHP) continue;
			if (augment->AugmentType == EAugmentType::AUG_Unit)
			{
				ApplyAugmentToUnit(augment->GE, rank);
			}
			else if(augment->AugmentType == EAugmentType::AUG_Util)
			{
				ApplyAugmentToSelf(augment->GE);
			}
		}
	}
}

void AURRBoard::SpawnUnit()
{
	//GA Activate
	if (!ASC) return;

	bool bPossible = false;
	for (auto row : Tiles)
	{
		for (auto tile : row)
		{
			if (tile->IsEmpty())
			{
				bPossible = true;
				break;
			}
		}
	}

	if (!bPossible) return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(0);
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

void AURRBoard::ApplyAugmentToUnit(TSubclassOf<UGameplayEffect> GE, int Target)
{
	if (!ASC) return;

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GE, 0, EffectContextHandle);

	for (auto& Row : Tiles)
	{
		for (auto& tile : Row)
		{
			if (Target != tile->GetRank()) continue;

			auto UnitCharacter = tile->UnitCharacter;
			if (!UnitCharacter) continue;

			UAbilitySystemComponent* TargetASC = UnitCharacter->GetAbilitySystemComponent();
			if (!TargetASC) continue;

			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, TargetASC);
		}
	}
}

void AURRBoard::ApplyAugmentToSelf(TSubclassOf<UGameplayEffect> GE)
{
	if (!ASC) return;

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GE, 0, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AURRBoard::SetCurrentMonsterWave(FMonsterInfo monster)
{
	if (HudWidget)
	{
		HudWidget->SetCurrentMosterInfo(monster);
	}
}

void AURRBoard::DeathMonster()
{
	if (HudWidget)
	{
		HudWidget->DeathMonster();
	}
}
