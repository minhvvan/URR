// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/URRAT_MoveWithSpline.h"
#include "Character/URRCharacterMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/URRMonsterSpawner.h"
#include "AbilitySystemComponent.h"
#include "Attribute/URRMonsterAttributeSet.h"
#include "Engine/World.h"
#include "URR.h"


UURRAT_MoveWithSpline::UURRAT_MoveWithSpline()
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
}

UURRAT_MoveWithSpline* UURRAT_MoveWithSpline::MoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UURRAT_MoveWithSpline* MyObj = NewAbilityTask<UURRAT_MoveWithSpline>(OwningAbility, TaskInstanceName);

	return MyObj;
}

void UURRAT_MoveWithSpline::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}


void UURRAT_MoveWithSpline::Activate()
{
}

void UURRAT_MoveWithSpline::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	AURRCharacterMonster* MyCharacter = Cast<AURRCharacterMonster>(GetAvatarActor());
	if (MyCharacter)
	{
		AURRMonsterSpawner* Spanwer = MyCharacter->GetSpawner();
		if (MyCharacter->GetActorLocation() == Spanwer->GetGatePos())
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnTargetLocationReached.Broadcast();
				EndTask();
			}
		}

		UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		if (CharMoveComp)
		{
			CharMoveComp->SetMovementMode(MOVE_Custom, 0);
		}

		UAbilitySystemComponent* ASC = MyCharacter->GetAbilitySystemComponent();
		if (ASC)
		{
			const UURRMonsterAttributeSet* URRMonsterAttributeSet = ASC->GetSet<UURRMonsterAttributeSet>();
			if (URRMonsterAttributeSet)
			{
				float Speed = URRMonsterAttributeSet->GetSpeed();
				float Distance = URRMonsterAttributeSet->GetDistance();
				Distance += DeltaTime * Speed;
				
				FTransform newTransform = Spanwer->GetPathTransform(Distance);

				ASC->ApplyModToAttribute(URRMonsterAttributeSet->GetDistanceAttribute(), EGameplayModOp::Override, Distance);
				MyCharacter->SetActorTransform(newTransform);
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UURRAT_MoveWithSpline::OnDestroy(bool AbilityIsEnding)
{
	ACharacter* MyCharacter = CastChecked<ACharacter>(GetAvatarActor());
	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
	if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
	{
		CharMoveComp->SetMovementMode(MOVE_Falling);
	}

	Super::OnDestroy(AbilityIsEnding);
}