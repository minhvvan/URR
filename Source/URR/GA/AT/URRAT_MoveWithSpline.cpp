// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/URRAT_MoveWithSpline.h"
#include "Character/URRCharacterMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/URRMonsterSpawner.h"
#include "AbilitySystemComponent.h"
#include "Attribute/MonsterAttributeSet.h"
#include "Engine/World.h"


UURRAT_MoveWithSpline::UURRAT_MoveWithSpline()
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
}

UURRAT_MoveWithSpline* UURRAT_MoveWithSpline::MoveToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location)
{
	UURRAT_MoveWithSpline* MyObj = NewAbilityTask<UURRAT_MoveWithSpline>(OwningAbility, TaskInstanceName);

	MyObj->GatePos = Location;
	return MyObj;
}

void UURRAT_MoveWithSpline::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
	
	Distance = 0;
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
		if (MyCharacter->GetActorLocation() == GatePos)
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

		const UMonsterAttributeSet* MonsterAttributeSet = MyCharacter->GetAbilitySystemComponent()->GetSet<UMonsterAttributeSet>();
		if (MonsterAttributeSet)
		{
			float Speed = MonsterAttributeSet->GetSpeed();
			Distance += DeltaTime * Speed;

			AURRMonsterSpawner* Spanwer = MyCharacter->GetSpawner();
			FTransform newTransform = Spanwer->GetPathTransform(Distance);

			MyCharacter->SetActorTransform(newTransform);
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
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
			{
				CharMoveComp->SetMovementMode(MOVE_Falling);
			}
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}
