// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterMonster.h"
#include "AbilitySystemComponent.h"
#include "Attribute/MonsterAttributeSet.h"
#include "AI/URRMonsterAIController.h"
#include "UI/URRGASWidgetComponent.h"
#include "UI/URRHudWidget.h"
#include "Engine/AssetManager.h"
#include "URR.h"

AURRCharacterMonster::AURRCharacterMonster() : MonsterID(0), bMove(false)
{
    MonsterAttributeSet = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("MonsterAttributeSet"));
	HpBarComp = CreateDefaultSubobject<UURRGASWidgetComponent>(TEXT("HpBarComp"));
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));

	WeaponMesh->SetupAttachment(RootComponent);
	ShieldMesh->SetupAttachment(RootComponent);
	HpBarComp->SetupAttachment(RootComponent);

	WeaponMesh->SetHiddenInGame(true);
	ShieldMesh->SetHiddenInGame(true);

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/URR/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (WidgetRef.Succeeded())
	{
		HpBarComp->SetWidgetClass(WidgetRef.Class);
		HpBarComp->SetDrawSize({ 50.f, 10.f });
		HpBarComp->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComp->SetRelativeLocation({ 0.f, 0.f, 220.f });
	}

    AIControllerClass = AURRMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    WeaponSocketName = FName(TEXT("SKT_Weapon"));
	ShieldSocketName = FName(TEXT("SKT_Shield"));

	NeedWeapon.Add(2);
	NeedWeapon.Add(3);
	NeedWeapon.Add(8);
	NeedWeapon.Add(9);
}

UAbilitySystemComponent* AURRCharacterMonster::GetAbilitySystemComponent() const
{
    return ASC;
}

void AURRCharacterMonster::InitMonster(int monsterID)
{
    MonsterID = monsterID;
}

void AURRCharacterMonster::StartMove()
{
	if (ASC)
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(0);
		if (Spec)
		{
			bMove = true;
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AURRCharacterMonster::SetSpawner(AURRMonsterSpawner* spawner)
{
	Spawner = spawner;
}

void AURRCharacterMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//URR_LOG(LogURR, Log, TEXT("%s"), *GetActorLocation().ToString());
}

void AURRCharacterMonster::BeginPlay()
{
    Super::BeginPlay();
}

void AURRCharacterMonster::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (ASC)
    {
        ASC->InitAbilityActorInfo(this, this);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, MonsterID + 1, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);

			for (auto StartAbility : StartAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartAbility.Value);
				StartSpec.InputID = StartAbility.Key;

				ASC->GiveAbility(StartSpec);
			}
		}
    }

    MonsterMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MonsterMeshes[MonsterID], FStreamableDelegate::CreateUObject(this, &AURRCharacterMonster::MonsterMeshLoadCompleted));
	AnimInstanceHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AnimInstances[MonsterID], FStreamableDelegate::CreateUObject(this, &AURRCharacterMonster::AnimInstanceLoadCompleted));
	
	for (int idx = 0; idx < NeedWeapon.Num(); idx++)
	{
		if (NeedWeapon[idx] == MonsterID)
		{
			WeaponMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WeaponMeshes[idx], FStreamableDelegate::CreateUObject(this, &AURRCharacterMonster::WeaponMeshLoadCompleted));
			break;
		}
	}

	if (MonsterID == 8)
	{
		ShieldMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ShieldMeshPath, FStreamableDelegate::CreateUObject(this, &AURRCharacterMonster::ShieldMeshLoadCompleted));
	}
}

void AURRCharacterMonster::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

}

void AURRCharacterMonster::MonsterMeshLoadCompleted()
{
	if (MonsterMeshHandle.IsValid())
	{
		USkeletalMesh* Monster = Cast<USkeletalMesh>(MonsterMeshHandle->GetLoadedAsset());
		if (Monster && GetMesh())
		{
			GetMesh()->SetSkeletalMesh(Monster);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	MonsterMeshHandle->ReleaseHandle();
}

void AURRCharacterMonster::WeaponMeshLoadCompleted()
{
	if (WeaponMeshHandle.IsValid())
	{
		UStaticMesh* Weapon = Cast<UStaticMesh>(WeaponMeshHandle->GetLoadedAsset());
		if (Weapon && GetMesh())
		{
			WeaponMesh->SetStaticMesh(Weapon);
			WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
			WeaponMesh->SetHiddenInGame(false);
		}
	}

	WeaponMeshHandle->ReleaseHandle();
}

void AURRCharacterMonster::AnimInstanceLoadCompleted()
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

void AURRCharacterMonster::ShieldMeshLoadCompleted()
{
	if (ShieldMeshHandle.IsValid())
	{
		UStaticMesh* Shield = Cast<UStaticMesh>(ShieldMeshHandle->GetLoadedAsset());
		if (Shield && GetMesh())
		{
			ShieldMesh->SetStaticMesh(Shield);
			ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ShieldSocketName);
			ShieldMesh->SetHiddenInGame(false);
		}
	}

	AnimInstanceHandle->ReleaseHandle();
}
