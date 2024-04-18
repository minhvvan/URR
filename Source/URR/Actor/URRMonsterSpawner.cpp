// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRMonsterSpawner.h"
#include "Character/URRCharacterMonster.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Character/URRCharacterMecha.h"
#include "AbilitySystemComponent.h"

// Sets default values
AURRMonsterSpawner::AURRMonsterSpawner()
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

	//UI Alert

	//Spawn
	SpawnMonster();
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

void AURRMonsterSpawner::MonsterDeathCallback(AURRCharacterMonster* monster)
{
	SpawnedMonsters.Remove(monster);

	if (SpawnedMonsters.Num() == 0)
	{
		SpawnMonster();
	}
}

// Called every frame
void AURRMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FMonsterInfo AURRMonsterSpawner::GetCurrentMonsterInfo()
{
	return MonsterWaves[currentIdx++];
}

void AURRMonsterSpawner::AddSpawnedMonster(AURRCharacterMonster* monster)
{
	//monster Die Delegate Bind

	SpawnedMonsters.Add(monster);
	monster->SetSpawner(this);
}

FTransform AURRMonsterSpawner::GetPathTransform(float DeltaTime)
{
	if (PathSpline)
	{
		return PathSpline->GetTransformAtDistanceAlongSpline(DeltaTime, ESplineCoordinateSpace::World);
	}

	return FTransform();
}
