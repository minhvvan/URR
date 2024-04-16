// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterUnit.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "URR.h"

AURRCharacterUnit::AURRCharacterUnit() : Rank(0)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_Soldier.SK_Soldier'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	UnitAttributeSet = CreateDefaultSubobject<UURRUnitAttributeSet>(TEXT("UnitAttributeSet"));
}

UAbilitySystemComponent* AURRCharacterUnit::GetAbilitySystemComponent() const
{
	return ASC;
}

void AURRCharacterUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AURRCharacterUnit::BeginPlay()
{
	Super::BeginPlay();
}

void AURRCharacterUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Rank + 1, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AURRCharacterUnit::Init(int rank)
{
	Rank = rank;
}

void AURRCharacterUnit::AttackMontageLoadCompleted()
{

}
