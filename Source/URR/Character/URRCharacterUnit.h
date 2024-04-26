// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "URRCharacterUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadCompleted);

UENUM()
enum class EAdditiveMeshEnum : uint8
{
	MESH_Helmet,
	MESH_Mask,
	MESH_BackPack,
	MESH_Mattress,
	MESH_BodyKit,
	MESH_Mecha_Back_01,
	MESH_Mecha_Back_02,
	MESH_Mecha_Back_03,
};

UENUM()
enum class EAssetType : uint8
{
	ASSET_UNIT_MESH,
	ASSET_MAT,
	ASSET_WEAPON,
	ASSET_HEAD,
	ASSET_BODY,
	ASSET_ACC,
	ASSET_ATTACK_MONTAGE,
	ASSET_PLACED_MONTAGE,
};

UCLASS(config = URR)
class URR_API AURRCharacterUnit : public AURRCharacterBase
{
	GENERATED_BODY()
	
public:
	AURRCharacterUnit();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> AccMesh;

protected:
	UPROPERTY()
	TObjectPtr<class UURRUnitAttributeSet> UnitAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int, TSubclassOf<class UGameplayAbility>> FindTargetAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int, TSubclassOf<class UGameplayAbility>> AttakAbilities;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Init(int rank);

	FORCEINLINE class UAnimMontage* GetPlacedActionMontage() const { return PlacedActionMontage; }
	FORCEINLINE class UAnimMontage* GetAttackMontage() const { return AttackActionMontage; }

	FOnLoadCompleted OnLoadCompleteDelegate;

protected:
	void UnitMeshLoadCompleted();
	void WeaponMeshLoadCompleted();
	void HelmetMeshLoadCompleted();
	void MaskMeshLoadCompleted();
	void BackPackMeshLoadCompleted();
	void MattressMeshLoadCompleted();
	void BodyKitMeshLoadCompleted();

	void UnitMaterialLoadCompleted();
	void HelmetMaterialLoadCompleted();
	void BodyKitMaterialLoadCompleted();
	void BackPackMaterialLoadCompleted();

	void AnimInstanceLoadCompleted();
	void AttackMontageLoadCompleted();
	void PlacedMontageLoadCompleted();

	virtual void UnitLoadCompleted(int part);

protected:
	int Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> PlacedActionMontage;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackActionMontage;

protected:
	TArray<bool> LoadCompletedPart;

	UPROPERTY(config)
	TArray<FSoftObjectPath> UnitMeshes;
	TSharedPtr<FStreamableHandle> UnitMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> WeaponMeshes;
	TSharedPtr<FStreamableHandle> WeaponMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AdditiveMeshes;
	TMap<int, TSharedPtr<FStreamableHandle>> AdditiveMeshHandles;

	UPROPERTY(config)
	TArray<FSoftObjectPath> UnitMaterials;
	TSharedPtr<FStreamableHandle> UnitMaterialHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> BodyKitMaterials;
	TSharedPtr<FStreamableHandle> BodyKitMaterialHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> BackPackMaterials;
	TSharedPtr<FStreamableHandle> BackPackMaterialHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AnimInstances;
	TSharedPtr<FStreamableHandle> AnimInstanceHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AttackMontages;
	TSharedPtr<FStreamableHandle> AttackMontageHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PlaceMontages;
	TSharedPtr<FStreamableHandle> PlaceMontageHandle;

	UPROPERTY(config)
	TArray<FName> SocketName;
};
