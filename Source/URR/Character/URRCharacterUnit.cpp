// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterUnit.h"
#include "Character/URRCharacterMonster.h"
#include "Actor/URRProjectile.h"
#include "Attribute/URRUnitAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "Tag/URRGameplayTag.h"
#include "URR.h"
#include "Kismet/KismetMathLibrary.h"

AURRCharacterUnit::AURRCharacterUnit() : Rank(0)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_Soldier.SK_Soldier'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetHiddenInGame(true);

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

	UnitAttributeSet = CreateDefaultSubobject<UURRUnitAttributeSet>(TEXT("UnitAttributeSet"));

	LoadCompletedPart.Init(false, 8);
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
	PrimaryActorTick.bCanEverTick = true;
}

void AURRCharacterUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		if (FindTargetAbilities[Rank])
		{
			FGameplayAbilitySpec FindTargetSpec(FindTargetAbilities[Rank]);
			FindTargetSpec.InputID = 1;
			ASC->GiveAbility(FindTargetSpec);
		}		
		
		if (AttakAbilities[Rank])
		{
			FGameplayAbilitySpec AttackSpec(AttakAbilities[Rank]);
			ASC->GiveAbility(AttackSpec);
		}

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

		FGameplayAbilitySpec* FindTargetSpec = ASC->FindAbilitySpecFromInputID(1);
		if (FindTargetSpec)
		{
			ASC->TryActivateAbility(FindTargetSpec->Handle);
		}
	}

	auto CharacterMesh = GetMesh();
	if (CharacterMesh)
	{
		switch (Rank)
		{
		case 4:
			CharacterMesh->SetWorldScale3D(FVector(2.1f, 2.1f, 2.1f));
			break;
		case 5:
			CharacterMesh->SetWorldScale3D(FVector(1.6f, 1.6f, 1.6f));
			break;
		case 6:
			CharacterMesh->SetWorldScale3D(FVector(1.3f, 1.3f, 1.3f));
			break;
		}
	}
}

void AURRCharacterUnit::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (TargetMonster && ASC->HasMatchingGameplayTag(URRTAG_UNIT_ATTACKING))
	{
		FVector TargetPos = TargetMonster->GetActorLocation();

		FRotator NewRot = GetActorRotation();
		NewRot.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPos).Yaw;

		SetActorRotation(NewRot);
	}
	else
	{
		FGameplayAbilitySpec* FindTargetSpec = ASC->FindAbilitySpecFromInputID(1);
		if (FindTargetSpec)
		{
			ASC->TryActivateAbility(FindTargetSpec->Handle);
		}
	}
}


void AURRCharacterUnit::Init(int rank)
{
	Rank = rank;

	int UnitMeshIdx = FMath::Clamp(Rank-3, 0, 7);
	int AnimInstanceIdx = FMath::Clamp(Rank - 3, 0, 4);

	UnitMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMeshes[UnitMeshIdx], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::UnitMeshLoadCompleted));
	UnitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(UnitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::UnitMaterialLoadCompleted));
	AnimInstanceHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AnimInstances[AnimInstanceIdx], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::AnimInstanceLoadCompleted));
	AttackMontageHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AttackMontages[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::AttackMontageLoadCompleted));
	PlaceMontageHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(PlaceMontages[AnimInstanceIdx], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::PlacedMontageLoadCompleted));

	switch (rank)
	{
	case 0:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::HelmetMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_MAT] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_BODY] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 1:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_Mask, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Mask], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::MaskMeshLoadCompleted)));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BodyKit, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_BodyKit], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BodyKitMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 2:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::HelmetMeshLoadCompleted)));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_BackPack], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_Mattress, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Mattress], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::MattressMeshLoadCompleted)));
		ProjectileClass = ProjectileClasses[0];
		break;
	case 3:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_Helmet, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Helmet], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::HelmetMeshLoadCompleted)));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_BackPack], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		ProjectileClass = ProjectileClasses[1];
		break;
	case 4:
		LoadCompletedPart[(int)EAssetType::ASSET_WEAPON] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_BODY] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 5:
		LoadCompletedPart[(int)EAssetType::ASSET_WEAPON] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_BODY] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		ProjectileClass = ProjectileClasses[1];
		break;
	case 6:
		LoadCompletedPart[(int)EAssetType::ASSET_WEAPON] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_BODY] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 7:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank-3], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Mecha_Back_01], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 8:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank-3], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Mecha_Back_02], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 9:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank-3], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_Mecha_Back_03], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	case 10:
		WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[Rank-3], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::WeaponMeshLoadCompleted));
		AdditiveMeshHandles.Add((int)EAdditiveMeshEnum::MESH_BackPack, UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AdditiveMeshes[(int)EAdditiveMeshEnum::MESH_BackPack], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMeshLoadCompleted)));
		LoadCompletedPart[(int)EAssetType::ASSET_HEAD] = true;
		LoadCompletedPart[(int)EAssetType::ASSET_ACC] = true;
		break;
	}
}

void AURRCharacterUnit::SetTargetMonster(AURRCharacterMonster* target)
{
	TargetMonster = target;
}

AURRCharacterMonster* AURRCharacterUnit::GetTargetMonster()
{
	return TargetMonster;
}

TSubclassOf<AURRProjectile> AURRCharacterUnit::GetProjectileClass()
{
	return ProjectileClass;
}

FTransform AURRCharacterUnit::GetMuzzleTransform()
{
	if (Rank == 2)
	{
		return GetMesh()->GetSocketTransform(MuzzleSocketName[0]);
	}
	else if(Rank == 3)
	{
		return GetMesh()->GetSocketTransform(MuzzleSocketName[1]);
	}
	else if (Rank == 5)
	{
		return GetMesh()->GetSocketTransform(MuzzleSocketName[2]);
	}

	return FTransform::Identity;
}

void AURRCharacterUnit::UnitMeshLoadCompleted()
{
	if (UnitMeshHandle.IsValid())
	{
		USkeletalMesh* UnitMesh = Cast<USkeletalMesh>(UnitMeshHandle->GetLoadedAsset());
		if (UnitMesh && GetMesh())
		{
			GetMesh()->SetSkeletalMesh(UnitMesh);
		}
	}

	UnitMeshHandle->ReleaseHandle();
	UnitLoadCompleted((int)EAssetType::ASSET_UNIT_MESH);
}

void AURRCharacterUnit::WeaponMeshLoadCompleted()
{
	if (WeaponMeshHandle.IsValid())
	{
		UStaticMesh* Weapon = Cast<UStaticMesh>(WeaponMeshHandle->GetLoadedAsset());
		if (Weapon && GetMesh())
		{
			int Idx = Rank;
			if (Idx > 6) Idx -= 3;

			WeaponMesh->SetStaticMesh(Weapon);
			WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName[Idx]);
			WeaponMesh->SetHiddenInGame(false);
		}
	}

	WeaponMeshHandle->ReleaseHandle();
	UnitLoadCompleted((int)EAssetType::ASSET_WEAPON);
}

void AURRCharacterUnit::HelmetMeshLoadCompleted()
{
	if (AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Helmet].IsValid())
	{
		USkeletalMesh* Helmet = Cast<USkeletalMesh>(AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Helmet]->GetLoadedAsset());
		if (Helmet && GetMesh())
		{
			HeadMesh->SetSkeletalMesh(Helmet);
			HeadMesh->SetHiddenInGame(false);

			BodyKitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyKitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::HelmetMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Helmet]->ReleaseHandle();
	AdditiveMeshHandles.Remove((int)EAdditiveMeshEnum::MESH_Helmet);
}

void AURRCharacterUnit::MaskMeshLoadCompleted()
{
	if (AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mask].IsValid())
	{
		USkeletalMesh* Mask = Cast<USkeletalMesh>(AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mask]->GetLoadedAsset());
		if (Mask && GetMesh())
		{
			HeadMesh->SetSkeletalMesh(Mask);
			HeadMesh->SetHiddenInGame(false);
		}
	}

	AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mask]->ReleaseHandle();
	AdditiveMeshHandles.Remove((int)EAdditiveMeshEnum::MESH_Mask);

	UnitLoadCompleted((int)EAssetType::ASSET_HEAD);
}

void AURRCharacterUnit::BackPackMeshLoadCompleted()
{
	if (AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BackPack].IsValid())
	{
		USkeletalMesh* BackPack = Cast<USkeletalMesh>(AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BackPack]->GetLoadedAsset());
		if (BackPack && GetMesh())
		{
			BodyMesh->SetSkeletalMesh(BackPack);
			BodyMesh->SetHiddenInGame(false);

			int Idx = Rank;
			if (Idx >= 7) Idx -= 3;
			BackPackMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BackPackMaterials[Idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BackPackMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BackPack]->ReleaseHandle();
	AdditiveMeshHandles.Remove((int)EAdditiveMeshEnum::MESH_BackPack);
}

void AURRCharacterUnit::MattressMeshLoadCompleted()
{
	if (AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mattress].IsValid())
	{
		USkeletalMesh* Matttress = Cast<USkeletalMesh>(AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mattress]->GetLoadedAsset());
		if (Matttress && GetMesh())
		{
			AccMesh->SetSkeletalMesh(Matttress);
			AccMesh->SetHiddenInGame(false);
		}
	}

	AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_Mattress]->ReleaseHandle();
	AdditiveMeshHandles.Remove((int)EAdditiveMeshEnum::MESH_Mattress);
	UnitLoadCompleted((int)EAssetType::ASSET_ACC);
}

void AURRCharacterUnit::BodyKitMeshLoadCompleted()
{
	if (AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BodyKit].IsValid())
	{
		USkeletalMesh* BodyKit = Cast<USkeletalMesh>(AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BodyKit]->GetLoadedAsset());
		if (BodyKit && GetMesh())
		{
			BodyMesh->SetSkeletalMesh(BodyKit);
			BodyMesh->SetHiddenInGame(false);

			BodyKitMaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyKitMaterials[Rank], FStreamableDelegate::CreateUObject(this, &AURRCharacterUnit::BodyKitMaterialLoadCompleted));
		}
	}

	AdditiveMeshHandles[(int)EAdditiveMeshEnum::MESH_BodyKit]->ReleaseHandle();
	AdditiveMeshHandles.Remove((int)EAdditiveMeshEnum::MESH_BodyKit);
}

void AURRCharacterUnit::UnitMaterialLoadCompleted()
{
	if (UnitMaterialHandle.IsValid())
	{
		UMaterialInstance* UnitMat = Cast<UMaterialInstance>(UnitMaterialHandle->GetLoadedAsset());
		if (UnitMat && GetMesh())
		{
			GetMesh()->SetMaterial(0, UnitMat);
			if (Rank >= 7 && WeaponMesh) WeaponMesh->SetMaterial(0, UnitMat);
		}
	}

	UnitMaterialHandle->ReleaseHandle();
	UnitLoadCompleted((int)EAssetType::ASSET_MAT);
}

void AURRCharacterUnit::HelmetMaterialLoadCompleted()
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
	UnitLoadCompleted((int)EAssetType::ASSET_HEAD);
}

void AURRCharacterUnit::BodyKitMaterialLoadCompleted()
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
	UnitLoadCompleted((int)EAssetType::ASSET_BODY);
}

void AURRCharacterUnit::BackPackMaterialLoadCompleted()
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
	UnitLoadCompleted((int)EAssetType::ASSET_BODY);
}

void AURRCharacterUnit::AnimInstanceLoadCompleted()
{
	if (AnimInstanceHandle.IsValid())
	{
		UClass* Anim = Cast<UClass>(AnimInstanceHandle->GetLoadedAsset());
		if (Anim && GetMesh())
		{
			GetMesh()->SetAnimInstanceClass(Anim);
		}
	}

	AnimInstanceHandle->ReleaseHandle();
}


void AURRCharacterUnit::AttackMontageLoadCompleted()
{
	if (AttackMontageHandle.IsValid())
	{
		AttackActionMontage = CastChecked<UAnimMontage>(AttackMontageHandle->GetLoadedAsset());
	}

	AttackMontageHandle->ReleaseHandle();
	UnitLoadCompleted((int)EAssetType::ASSET_ATTACK_MONTAGE);
}

void AURRCharacterUnit::PlacedMontageLoadCompleted()
{
	if (PlaceMontageHandle.IsValid())
	{
		PlacedActionMontage = CastChecked<UAnimMontage>(PlaceMontageHandle->GetLoadedAsset());
	}

	PlaceMontageHandle->ReleaseHandle();
	UnitLoadCompleted((int)EAssetType::ASSET_PLACED_MONTAGE);
}

void AURRCharacterUnit::UnitLoadCompleted(int part)
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