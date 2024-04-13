// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterRobot.h"

AURRCharacterRobot::AURRCharacterRobot()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Battle_Robot/SK_BattleRobot.SK_BattleRobot'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
}
