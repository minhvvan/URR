// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterSoldier.h"
#include "Engine/AssetManager.h"

enum EAdditiveMeshEnum
{
	MESH_Helmet,
	MESH_Mask,
	MESH_BackPack,
	MESH_Mattress,
	MESH_BodyKit,
};

AURRCharacterSoldier::AURRCharacterSoldier()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> UnitMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_Soldier.SK_Soldier'"));
	if (UnitMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(UnitMeshRef.Object);
	}

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetLeaderPoseComponent(GetMesh());
	HeadMesh->SetHiddenInGame(true);

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetLeaderPoseComponent(GetMesh());
	BodyMesh->SetHiddenInGame(true);

	AccMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AccMesh"));
	AccMesh->SetupAttachment(RootComponent);
	AccMesh->SetLeaderPoseComponent(GetMesh());
	AccMesh->SetHiddenInGame(true);
}

void AURRCharacterSoldier::Init(int rank)
{
	Super::Init(rank);

	WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::WeaponMeshLoadCompleted));
	UnitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::UnitMaterialLoadCompleted));

	switch (rank)
	{
	case 0:
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::HelmetMeshLoadCompleted)));
		break;
	case 1:
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_Mask, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_Mask], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::MaskMeshLoadCompleted)));
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_BodyKit, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_BodyKit], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::BodyKitMeshLoadCompleted)));
		break;
	case 2:
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::HelmetMeshLoadCompleted)));
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_BackPack], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::BackPackMeshLoadCompleted)));
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_Mattress, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_Mattress], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::MattressMeshLoadCompleted)));
		break;
	case 3:
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::HelmetMeshLoadCompleted)));
		AdditiveMeshHandles.Add(EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[EAdditiveMeshEnum::MESH_BackPack], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::BackPackMeshLoadCompleted)));
		break;
	}
}

void AURRCharacterSoldier::BeginPlay()
{
	Super::BeginPlay();
}

void AURRCharacterSoldier::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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

void AURRCharacterSoldier::HelmetMeshLoadCompleted()
{
	if (AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Helmet].IsValid())
	{
		USkeletalMesh* Helmet = Cast<USkeletalMesh>(AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Helmet]->GetLoadedAsset());
		if (Helmet && GetMesh())
		{
			HeadMesh->SetSkeletalMesh(Helmet);
			HeadMesh->SetHiddenInGame(false);

			BodyKitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyKitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::HelmetMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Helmet]->ReleaseHandle();
	AdditiveMeshHandles.Remove(EAdditiveMeshEnum::MESH_Helmet);
}

void AURRCharacterSoldier::MaskMeshLoadCompleted()
{
	if (AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mask].IsValid())
	{
		USkeletalMesh* Mask = Cast<USkeletalMesh>(AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mask]->GetLoadedAsset());
		if (Mask && GetMesh())
		{
			HeadMesh->SetSkeletalMesh(Mask);
			HeadMesh->SetHiddenInGame(false);
		}
	}

	AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mask]->ReleaseHandle();
	AdditiveMeshHandles.Remove(EAdditiveMeshEnum::MESH_Mask);
}

void AURRCharacterSoldier::BackPackMeshLoadCompleted()
{
	if (AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BackPack].IsValid())
	{
		USkeletalMesh* BackPack = Cast<USkeletalMesh>(AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BackPack]->GetLoadedAsset());
		if (BackPack && GetMesh())
		{
			BodyMesh->SetSkeletalMesh(BackPack);
			BodyMesh->SetHiddenInGame(false);

			BackPackMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BackPackMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::BackPackMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BackPack]->ReleaseHandle();
	AdditiveMeshHandles.Remove(EAdditiveMeshEnum::MESH_BackPack);
}

void AURRCharacterSoldier::MattressMeshLoadCompleted()
{
	if (AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mattress].IsValid())
	{
		USkeletalMesh* Matttress = Cast<USkeletalMesh>(AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mattress]->GetLoadedAsset());
		if (Matttress && GetMesh())
		{
			AccMesh->SetSkeletalMesh(Matttress);
			AccMesh->SetHiddenInGame(false);
		}
	}

	AdditiveMeshHandles[EAdditiveMeshEnum::MESH_Mattress]->ReleaseHandle();
	AdditiveMeshHandles.Remove(EAdditiveMeshEnum::MESH_Mattress);
}

void AURRCharacterSoldier::BodyKitMeshLoadCompleted()
{
	if (AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BodyKit].IsValid())
	{
		USkeletalMesh* BodyKit = Cast<USkeletalMesh>(AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BodyKit]->GetLoadedAsset());
		if (BodyKit && GetMesh())
		{
			BodyMesh->SetSkeletalMesh(BodyKit);
			BodyMesh->SetHiddenInGame(false);

			BodyKitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyKitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterSoldier::BodyKitMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[EAdditiveMeshEnum::MESH_BodyKit]->ReleaseHandle();
	AdditiveMeshHandles.Remove(EAdditiveMeshEnum::MESH_BodyKit);
}

void AURRCharacterSoldier::UnitMaterialLoadCompleted()
{
	if (UnitMaterialHandle.IsValid())
	{
		UMaterialInstance* UnitMat = Cast<UMaterialInstance>(UnitMaterialHandle->GetLoadedAsset());
		if (UnitMat && GetMesh())
		{
			GetMesh()->SetMaterial(0, UnitMat);
		}
	}

	UnitMaterialHandle->ReleaseHandle();
}

void AURRCharacterSoldier::HelmetMaterialLoadCompleted()
{
	if (BodyKitMaterialHandle.IsValid())
	{
		UMaterialInstance* BodyKitMat = Cast<UMaterialInstance>(BodyKitMaterialHandle->GetLoadedAsset());
		if (BodyKitMat && HeadMesh)
		{
			HeadMesh->SetMaterial(0, BodyKitMat);
		}
	}

	BodyKitMaterialHandle->ReleaseHandle();
}

void AURRCharacterSoldier::BodyKitMaterialLoadCompleted()
{
	if (BodyKitMaterialHandle.IsValid())
	{
		UMaterialInstance* BodyKitMat = Cast<UMaterialInstance>(BodyKitMaterialHandle->GetLoadedAsset());
		if (BodyKitMat && BodyMesh)
		{
			BodyMesh->SetMaterial(0, BodyKitMat);
		}
	}

	BodyKitMaterialHandle->ReleaseHandle();
}

void AURRCharacterSoldier::BackPackMaterialLoadCompleted()
{
	if (BackPackMaterialHandle.IsValid())
	{
		UMaterialInstance* BackPackMat = Cast<UMaterialInstance>(BackPackMaterialHandle->GetLoadedAsset());
		if (BackPackMat && BodyMesh)
		{
			BodyMesh->SetMaterial(0, BackPackMat);
		}
	}

	BackPackMaterialHandle->ReleaseHandle();
}
