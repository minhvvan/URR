// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "URRStageBlock.generated.h"

UENUM(BlueprintType)
enum class EBlockMat : uint8
{
	LOCKED UMETA(DisplayName = "Locked"),
	CLEAR UMETA(DisplayName = "Clear"),
	CURRENT UMETA(DisplayName = "Current"),
	SIZE UMETA(DisplayName = "Size"),
};

UCLASS()
class URR_API AURRStageBlock : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UURRStageWidget> StageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UURRStageWidget> StageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mat)
	TMap<EBlockMat, TObjectPtr<UMaterialInstance>> BlockMats;

public:	
	// Sets default values for this actor's properties
	AURRStageBlock();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, Category = Stage)
	int stageNum;

	UPROPERTY(EditAnywhere, Category = Stage)
	int stageScore;

	UPROPERTY(EditAnywhere, Category = Stage)
	bool bComplete;

	UPROPERTY(EditAnywhere, Category = Stage)
	bool bCurrent;
};
