// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "URRMonsterSpawner.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MonsterID;

	UPROPERTY(EditAnywhere)
	int MonsterNum;
};

UCLASS()
class URR_API AURRMonsterSpawner : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AURRMonsterSpawner();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoundBox;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class USplineComponent> PathSpline;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int, TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere)
	TArray<FMonsterInfo> MonsterWaves;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class AURRCharacterMonster>> SpawnedMonsters;

	int currentIdx;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMonster();

	UFUNCTION()
	void MonsterDeathCallback(AURRCharacterMonster* monster);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FMonsterInfo GetCurrentMonsterInfo();

	void AddSpawnedMonster(AURRCharacterMonster* monster);

	FTransform GetPathTransform(float DeltaTime);
};
