// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterUnit.h"
#include "URRCharacterMecha.generated.h"

/**
 * 
 */
UCLASS(config=URR)
class URR_API AURRCharacterMecha : public AURRCharacterUnit
{
	GENERATED_BODY()
	
public:
	AURRCharacterMecha();

	virtual void Init(int rank) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> AdditiveMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInstance> MasterMat;

protected:
	UPROPERTY(config)
	TArray<FSoftObjectPath> UnitMeshes;
	TSharedPtr<FStreamableHandle> UnitMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> WeaponMeshes;
	TSharedPtr<FStreamableHandle> WeaponMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> AdditiveMeshes;
	TSharedPtr<FStreamableHandle> AdditiveMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> UnitMaterials;
	TSharedPtr<FStreamableHandle> UnitMaterialHandle;

	UPROPERTY(config)
	FName BackPackSocketName;

protected:
	void UnitMeshLoadCompleted();
	void WeaponMeshLoadCompleted();
	void AdditiveMeshLoadCompleted();

	void UnitMaterialLoadCompleted();
};
