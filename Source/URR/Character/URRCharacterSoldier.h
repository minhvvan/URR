// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterUnit.h"
#include "URRCharacterSoldier.generated.h"

/**
 * 
 */

UCLASS(config = URR)
class URR_API AURRCharacterSoldier : public AURRCharacterUnit
{
	GENERATED_BODY()
	
public:
	AURRCharacterSoldier();

	virtual void Init(int rank) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category=Mesh)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> AccMesh;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

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

	void UnitLoadCompleted(int part);

protected:
	TArray<bool> LoadCompletedPart;

protected:
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
};