// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRMonsterSpawner.h"
#include "Character/URRCharacterMonster.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "AbilitySystemComponent.h"
#include "Tag/URRGameplayTag.h"

// Sets default values
AURRMonsterSpawner::AURRMonsterSpawner():
	currentIdx(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	
	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundBox"));
	SetRootComponent(BoundBox);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->SetupAttachment(RootComponent);

	BoundBox->SetBoxExtent({ 300, 300 ,300 });

	currentIdx = 0;
}

UAbilitySystemComponent* AURRMonsterSpawner::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AURRMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	//UI Alert

	//Spawn
	//SpawnMonster();
}

void AURRMonsterSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		for (auto StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility.Value);
			StartSpec.InputID = StartAbility.Key;

			ASC->GiveAbility(StartSpec);
		}
	}
}

void AURRMonsterSpawner::SetWaveInfo(TArray<FMonsterInfo> Waves)
{
	MonsterWaves = Waves;
}

void AURRMonsterSpawner::SpawnMonster()
{
	if (!ASC) return;

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(0);
	if (Spec)
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

void AURRMonsterSpawner::MonsterDeathCallback(AActor* monster)
{
	AURRCharacterMonster* Target = Cast<AURRCharacterMonster>(monster);
	SpawnedMonsters.Remove(Target);
}

// Called every frame
void AURRMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedMonsters.Num() == 0 && !ASC->HasMatchingGameplayTag(URRTAG_MONSTER_SPAWNING))
	{
		SpawnMonster();
	}
}

FMonsterInfo AURRMonsterSpawner::GetCurrentMonsterInfo()
{
	if (MonsterWaves.IsValidIndex(currentIdx))
	{
		return MonsterWaves[currentIdx++];
	}
	else
	{
		//Clear Stage
		return FMonsterInfo();
	}
}

void AURRMonsterSpawner::AddSpawnedMonster(AURRCharacterMonster* monster)
{
	//monster Die Delegate Bind
	monster->OnDestroyed.AddDynamic(this, &AURRMonsterSpawner::MonsterDeathCallback);

	SpawnedMonsters.Add(monster);
	monster->SetSpawner(this);
}

FTransform AURRMonsterSpawner::GetPathTransform(float Distance)
{
	if (PathSpline)
	{
		return PathSpline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	}

	return FTransform();
}

FVector AURRMonsterSpawner::GetGatePos()
{
	return PathSpline->GetWorldLocationAtSplinePoint(5);
}
