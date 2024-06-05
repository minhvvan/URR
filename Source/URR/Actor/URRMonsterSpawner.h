// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Framework/URRGameMode.h"
#include "Framework/URRWaveManager.h"
#include "URRMonsterSpawner.generated.h"

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

	UPROPERTY(EditAnywhere, Category = Spawn)
	TArray<FMonsterInfo> MonsterWaves;

	UPROPERTY(VisibleAnywhere, Category = Spawn)
	TArray<TObjectPtr<class AURRCharacterMonster>> SpawnedMonsters;

	UPROPERTY()
	TObjectPtr<class UURRWaveManager> WaveManager;

	int currentIdx;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UURRWaveAlertWidget> WaveAlertWidgetClass;

	TObjectPtr<UURRWaveAlertWidget> WaveAlertWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UURRStageClearWidget> StageClearWidgetClass;

	TObjectPtr<UURRStageClearWidget> StageClearWidget;

	TObjectPtr<class AURRBoard> Board;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

	void SetWaveInfo(TArray<FMonsterInfo> Waves);
	void SpawnMonster();

	UFUNCTION()
	void WaveAlertCallback();

	UFUNCTION()
	void MonsterDeathCallback(AActor* monster);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FMonsterInfo GetCurrentMonsterInfo(int increase);

	void AddSpawnedMonster(AURRCharacterMonster* monster);

	FTransform GetPathTransform(float Distance);

	FVector GetGatePos();

	friend UURRWaveManager;

};
