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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ShieldMesh;

	UPROPERTY(EditAnywhere, Category=UI)
	TObjectPtr<class UURRGASWidgetComponent> HpBarComp;

	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;	
	
	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

public:
	void InitMonster(int monsterID, float rewardLevel);

	void StartMove();
	void StartAttack();
	void SetSpawner(class AURRMonsterSpawner* spawner);
	AURRMonsterSpawner* GetSpawner() { return Spawner; }
	UAnimMontage* GetAttackMontage();

	UFUNCTION(BlueprintCallable)
	bool GetbMove() { return bMove; }

	void SetbMove(bool newMove) { bMove = newMove; }

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOutOfHealth();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UURRMonsterAttributeSet> URRMonsterAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;	
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitRewardEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GiveCoinffect;

	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMeshes;
	TSharedPtr<FStreamableHandle> MonsterMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> WeaponMeshes;
	TSharedPtr<FStreamableHandle> WeaponMeshHandle;

	UPROPERTY(config)
	FSoftObjectPath ShieldMeshPath;
	TSharedPtr<FStreamableHandle> ShieldMeshHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> MonsterMaterials;
	TSharedPtr<FStreamableHandle> MonsterMaterialHandle;
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> AnimInstances;
	TSharedPtr<FStreamableHandle> AnimInstanceHandle;

	UPROPERTY(config)
	TArray<FSoftObjectPath> DeadMontages;
	TSharedPtr<FStreamableHandle> DeadMontageHandle;	
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> AttackMontages;
	TSharedPtr<FStreamableHandle> AttackMontageHandle;

	FName WeaponSocketName;
	FName ShieldSocketName;

	UPROPERTY()
	TObjectPtr<AURRMonsterSpawner> Spawner;

	UPROPERTY(EditAnywhere, Category=Spawn)
	TArray<int> NeedWeapon;

protected:
	int MonsterID;
	bool bMove;
	int RewardLevel;

protected:
	void MonsterMeshLoadCompleted();
	void WeaponMeshLoadCompleted();
	void AnimInstanceLoadCompleted();
	void ShieldMeshLoadCompleted();
	void DeadMontageLoadCompleted();
	void AttackMontageLoadCompleted();

	void SetDead();
	void PlayDeadAnimation();

	UFUNCTION()
	void MontageEndCallback(UAnimMontage* Montage, bool bInterrupted);
};