// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterTank.h"

AURRCharacterTank::AURRCharacterTank()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Tank/SK_Tank.SK_Tank'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
}
