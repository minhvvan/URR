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
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void UnitMeshLoadCompleted();
	void WeaponMeshLoadCompleted();

	UPROPERTY(config)
	FSoftObjectPath UnitMesh;
	TSharedPtr<FStreamableHandle> UnitMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> WeaponMeshes;
	TSharedPtr<FStreamableHandle> WeaponMeshHandle;

	UPROPERTY(config)
	FSoftObjectPath HelmetMesh;
	TSharedPtr<FStreamableHandle> HelmetMeshHandle;

	UPROPERTY(config)
	FSoftObjectPath MaskMesh;
	TSharedPtr<FStreamableHandle> MaskMeshHandle;

	UPROPERTY(config)
	FSoftObjectPath BackPackMesh;
	TSharedPtr<FStreamableHandle> BackPackMeshHandle;

	UPROPERTY(config)
	FSoftObjectPath MattressMesh;
	TSharedPtr<FStreamableHandle> MattressMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> UnitMaterials;
	TSharedPtr<FStreamableHandle> UnitMaterialHandle;
};
