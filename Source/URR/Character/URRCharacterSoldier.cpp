// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterSoldier.h"
#include "Engine/AssetManager.h"

AURRCharacterSoldier::AURRCharacterSoldier()
{
}

void AURRCharacterSoldier::Init(int rank)
{
	Super::Init(rank);

	UnitMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMesh, FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::UnitMeshLoadCompleted));
}

void AURRCharacterSoldier::BeginPlay()
{
	Super::BeginPlay();
}

void AURRCharacterSoldier::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AURRCharacterSoldier::UnitMeshLoadCompleted()
{
	if (UnitMeshHandle.IsValid())
	{
		USkeletalMesh* SoldierMesh = Cast<USkeletalMesh>(UnitMeshHandle->GetLoadedAsset());
		if (SoldierMesh)
		{
			GetMesh()->SetSkeletalMesh(SoldierMesh);
			GetMesh()->SetHiddenInGame(false);

			if (Rank < 4)
			{
				WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::WeaponMeshLoadCompleted));
			}
		}
	}

	UnitMeshHandle->ReleaseHandle();
}

void AURRCharacterSoldier::WeaponMeshLoadCompleted()
{
	if (WeaponMeshHandle.IsValid())
	{
		UStaticMesh* Weapon = Cast<UStaticMesh>(WeaponMeshHandle->GetLoadedAsset());
		if (Weapon && GetMesh())
		{
			WeaponMesh->SetStaticMesh(Weapon);
			WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName[Rank]);
			WeaponMesh->SetHiddenInGame(false);
		}
	}

	WeaponMeshHandle->ReleaseHandle();
}