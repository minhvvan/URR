// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "URRTile.generated.h"

UCLASS(config = URR)
class URR_API AURRTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AURRTile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Unit, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AURRCharacterUnit> UnitCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Unit, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AURRCharacterUnit> UnitClass;

	UPROPERTY(config)
	TArray<FSoftClassPath> UnitClasses;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	bool isEmpty;
	int Rank;

public:
	bool IsEmpty();
	void SpawnUnit(int rank);

	void UnitClassLoadCompleted();
};
