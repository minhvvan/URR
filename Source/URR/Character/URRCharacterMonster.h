// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "URRCharacterMonster.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRCharacterMonster : public AURRCharacterBase
{
	GENERATED_BODY()
	
public:
	AURRCharacterMonster();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category=UI)
	TObjectPtr<class UURRGASWidgetComponent> HpBarComp;

public:
	void InitMonster(int monsterID);

	void StartMove();
	void SetSpawner(class AURRMonsterSpawner* spawner);
	AURRMonsterSpawner* GetSpawner() { return Spawner; }

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY()
	TObjectPtr<class UMonsterAttributeSet> MonsterAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMeshes;
	TSharedPtr<FStreamableHandle> MonsterMeshHandle;

	UPROPERTY(config)
	TMap<int, FSoftObjectPath> WeaponMeshes;
	TSharedPtr<FStreamableHandle> WeaponMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMaterials;
	TSharedPtr<FStreamableHandle> MonsterMaterialHandle;	
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> AnimInstances;
	TSharedPtr<FStreamableHandle> AnimInstanceHandle;

	FName WeaponSocketName;

	UPROPERTY()
	TObjectPtr<AURRMonsterSpawner> Spawner;

protected:
	int MonsterID;

protected:
	void MonsterMeshLoadCompleted();
	void WeaponMeshLoadCompleted();
	void AnimInstanceLoadCompleted();
};