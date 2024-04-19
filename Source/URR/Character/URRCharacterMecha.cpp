// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterMecha.h"
#include "Engine/AssetManager.h"

AURRCharacterMecha::AURRCharacterMecha()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> UnitMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ModularSciFiHeroesHP/Mesh/Mecha/Character/SK_Mecha_Pistol.SK_Mecha_Pistol'"));
	if (UnitMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(UnitMeshRef.Object);
	}

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	AdditiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditiveMesh"));
	AdditiveMesh->SetupAttachment(RootComponent);

	LoadCompletedPart.Init(false, 4);
}

void AURRCharacterMecha::Init(int rank)
{
	Super::Init(rank);
	int Idx = rank - 7;

	WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterMecha::WeaponMeshLoadCompleted));
	UnitMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMeshes[Idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterMecha::UnitMeshLoadCompleted));
	UnitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMaterials[Idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterMecha::UnitMaterialLoadCompleted));

	if (Idx == 1)
	{
		UnitLoadCompleted(LOAD_ADDITIVE);
		return;
	}
	
	Idx = FMath::Clamp(Idx-1, 0, 2);
	AdditiveMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[Idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterMecha::AdditiveMeshLoadCompleted));
}

void AURRCharacterMecha::UnitMeshLoadCompleted()
{
	if (UnitMeshHandle.IsValid())
	{
		USkeletalMesh* UnitMesh = Cast<USkeletalMesh>(UnitMeshHandle->GetLoadedAsset());
		if (UnitMesh && GetMesh())
		{
			GetMesh()->SetSkeletalMesh(UnitMesh);
		}
	}

	WeaponMeshHandle->ReleaseHandle();
	UnitLoadCompleted(LOAD_UNIT_MESH);
}

void AURRCharacterMecha::WeaponMeshLoadCompleted()
{
	if (WeaponMeshHandle.IsValid())
	{
		UStaticMesh* Weapon = Cast<UStaticMesh>(WeaponMeshHandle->GetLoadedAsset());
		if (Weapon && GetMesh())
		{
			int Idx = Rank - 7;

			WeaponMesh->SetStaticMesh(Weapon);
			WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName[Idx]);
			WeaponMesh->SetHiddenInGame(false);
		}
	}

	WeaponMeshHandle->ReleaseHandle();
	UnitLoadCompleted(ASSET_WEAPON);
}

void AURRCharacterMecha::AdditiveMeshLoadCompleted()
{
	if (AdditiveMeshHandle.IsValid())
	{
		UStaticMesh* BackPack = Cast<UStaticMesh>(AdditiveMeshHandle->GetLoadedAsset());
		if (BackPack && GetMesh())
		{
			AdditiveMesh->SetStaticMesh(BackPack);
			AdditiveMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, BackPackSocketName);
			AdditiveMesh->SetHiddenInGame(false);
		}
	}

	AdditiveMeshHandle->ReleaseHandle();
	UnitLoadCompleted(LOAD_ADDITIVE);
}

void AURRCharacterMecha::UnitMaterialLoadCompleted()
{
	if (UnitMaterialHandle.IsValid())
	{
		UMaterialInstance* Mat = Cast<UMaterialInstance>(UnitMaterialHandle->GetLoadedAsset());
		if (Mat)
		{
			MasterMat = Mat;

			if (GetMesh()) GetMesh()->SetMaterial(0, Mat);
			if (WeaponMesh) WeaponMesh->SetMaterial(0, Mat);
			if (AdditiveMesh) AdditiveMesh->SetMaterial(0, Mat);
		}
	}

	UnitMaterialHandle->ReleaseHandle();
	UnitLoadCompleted(ASSET_MAT);
}

void AURRCharacterMecha::UnitLoadCompleted(int part)
{
	LoadCompletedPart[part] = true;

	bool bComplete = true;
	for (auto Unitpart : LoadCompletedPart)
	{
		if (!Unitpart)
		{
			bComplete = false;
			break;
		}
	}

	if (bComplete) OnLoadCompleteDelegate.Broadcast();
}
