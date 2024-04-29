// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRMine.h"
#include "URR.h"

AURRMine::AURRMine()
{
}

void AURRMine::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	URR_LOG(LogURR, Log, TEXT("Overalp"));
	//Ground -> ¼³Ä¡

	//Monster -> Boom
}