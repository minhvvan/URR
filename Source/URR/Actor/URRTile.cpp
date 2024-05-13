// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRTile.h"
#include "Character/URRCharacterUnit.h"
#include "UI/URRGASWidgetComponent.h"
#include "UI/URRTileRankWidget.h"
#include "Engine/AssetManager.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "Urr.h"

// Sets default values
AURRTile::AURRTile(): isEmpty(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	SetRootComponent(TileMesh);	
	
	RankWidgetComp = CreateDefaultSubobject<UURRGASWidgetComponent>(TEXT("RankWidgetComp"));
	RankWidgetComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> RankWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/URR/UI/WBP_TileRank.WBP_TileRank_C'"));
	if (RankWidgetRef.Succeeded())
	{
		RankWidgetComp->SetWidgetClass(RankWidgetRef.Class);
		RankWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		RankWidgetComp->SetDrawSize(FVector2D(1000.f, 1000.f));
		RankWidgetComp->SetRelativeLocation({0, 0, 51.f});
		RankWidgetComp->SetRelativeRotation({90, 0, 180.f});
		RankWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RankWidgetComp->SetHiddenInGame(true);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/URR/Mesh/TileMesh.TileMesh'"));
	if (TileMeshRef.Succeeded())
	{
		TileMesh->SetStaticMesh(TileMeshRef.Object);
	}

	ConstructorHelpers::FClassFinder<AURRCharacterUnit> UnitClassRef(TEXT("/Script/Engine.Blueprint'/Game/URR/Blueprint/BP_Unit.BP_Unit_C'"));
	if (TileMeshRef.Succeeded())
	{
		UnitClass = UnitClassRef.Class;
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
	FTransform FinalTransform = FTransform(FRotator(0.f, 180.f, 0.f), SpawnLoc);
	UnitCharacter->FinishSpawning(FinalTransform);
}

void AURRTile::TileMaterialLoadCompleted()
{
	UMaterialInstance* Mat = Cast<UMaterialInstance>(TileMaterialHandle->GetLoadedAsset());
	if (Mat)
	{
		TileMesh->SetMaterial(0, Mat);
	}

	TileMaterialHandle->ReleaseHandle();
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

	if(!RankWidget) RankWidget = CastChecked<UURRTileRankWidget>(RankWidgetComp->GetWidget());
	Rank = rank;
	RankWidget->SetRank(rank);
	RankWidgetComp->SetHiddenInGame(false);

	TileMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(TileMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRTile::TileMaterialLoadCompleted));

	if (UnitClass)
	{
		//Spawn Uit
		FVector SpawnLoc = GetActorLocation();
		SpawnLoc.Z += 138;

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

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

		if (!RankWidget) RankWidget = CastChecked<UURRTileRankWidget>(RankWidgetComp->GetWidget());
		RankWidget->SetRank(0);
		RankWidgetComp->SetHiddenInGame(true);

		TileMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(TileMaterials[11], FStreamableDelegate::CreateUObject(this, &AURRTile::TileMaterialLoadCompleted));
	}
}

int AURRTile::GetRank()
{
	return Rank;
}

void AURRTile::ApplyAugment(TSubclassOf<UGameplayEffect> GE)
{
	if (!UnitCharacter) return;

	UAbilitySystemComponent* ASC = UnitCharacter->GetAbilitySystemComponent();
	if (!ASC) return;

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	ASC->BP_ApplyGameplayEffectToSelf(GE, 0, EffectContext);
	URR_LOG(LogURR, Log, TEXT("ApplyAugment"));
}