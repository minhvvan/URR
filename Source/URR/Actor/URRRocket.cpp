// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRRocket.h"
#include "URR.h"

void AURRRocket::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	URR_LOG(LogURR, Log, TEXT("Overalp"));
}
