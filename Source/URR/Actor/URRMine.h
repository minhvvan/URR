// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/URRProjectile.h"
#include "URRMine.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRMine : public AURRProjectile
{
	GENERATED_BODY()
	
public:
	AURRMine();

protected:
	virtual void BeginOverlapCallback(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
