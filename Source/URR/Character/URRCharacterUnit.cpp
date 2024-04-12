// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterUnit.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "URR.h"

AURRCharacterUnit::AURRCharacterUnit()
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

void AURRCharacterUnit::BeginPlay()
{
	Super::BeginPlay();

	URR_LOG(LogURR, Log, TEXT("BeginPlay"));
}

void AURRCharacterUnit::Init(int rank)
{
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, rank, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		URR_LOG(LogURR, Log, TEXT("Init"));
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}