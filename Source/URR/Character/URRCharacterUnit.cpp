// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterUnit.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "Tag/URRGameplayTag.h"
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
	AttackMontageHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AttackMontages[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::AttackMontageLoadCompleted));

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

		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(0);
		if (Spec)
		{
			ASC->TryActivateAbility(Spec->Handle);
		}

		FGameplayAbilitySpec* AttackSpec = ASC->FindAbilitySpecFromInputID(1);
		if (AttackSpec)
		{
			ASC->TryActivateAbility(AttackSpec->Handle);
		}
	}
}

void AURRCharacterUnit::Init(int rank)
{
	Rank = rank;
}

void AURRCharacterUnit::AttackMontageLoadCompleted()
{
	if (AttackMontageHandle.IsValid())
	{
		AttackActionMontage = CastChecked<UAnimMontage>(AttackMontageHandle->GetLoadedAsset());
	}

	AttackMontageHandle->ReleaseHandle();
}

void AURRCharacterUnit::UnitLoadCompleted(int part)
{
}