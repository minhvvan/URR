// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "URRTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompleteSpawnUnit);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UURRGASWidgetComponent> RankWidgetComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UnitLoadCompleteCallback();

	void TileMaterialLoadCompleted();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Unit, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AURRCharacterUnit> UnitClass;

	UPROPERTY(config)
	TArray<FSoftClassPath> TileMaterials;
	TSharedPtr<FStreamableHandle> TileMaterialHandle;

	UPROPERTY()
	TObjectPtr<class UURRTileRankWidget> RankWidget;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> RankUpSFX;

	bool isEmpty;
	int Rank;

protected:
	void SetTileRankMat();

public:
	FCompleteSpawnUnit OnCompleteSpawnUnitDelegate;

	bool IsEmpty();
	void SpawnUnit(int rank);
	void RankUpUnit();

	void DestroyUnit();

	int GetRank();

	void ApplyAugment(TSubclassOf<class UGameplayEffect> GE);

	void AdjustUnit();

	friend class AURRBoard;
};