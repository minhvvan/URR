// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterTurret.h"

AURRCharacterTurret::AURRCharacterTurret()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Turret/SK_Turret.SK_Turret'"));
	if(MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
}

void AURRCharacterTurret::Init(int rank)
{
	OnLoadCompleteDelegate.Broadcast();
}
