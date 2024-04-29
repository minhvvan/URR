// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRMine.h"
#include "Components/SphereComponent.h"
#include "Character/URRCharacterMonster.h"
#include "Attribute/URRProjectileAttributeSet.h"
#include "URR.h"
#include "Physics/URRCollision.h"

AURRMine::AURRMine()
{
}

UAbilitySystemComponent* AURRMine::GetAbilitySystemComponent() const
{
	return ASC;
}

void AURRMine::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Capsule->OnComponentHit.AddDynamic(this, &AURRMine::OnHitCallback);
}

void AURRMine::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	//URR_LOG(LogURR, Log, TEXT("Overalp: %s"), *OtherActor->GetName());
	//Monster -> Boom

	float AttackRange = ProjectileAttributeSet->GetAttackRange();
	TArray<FOverlapResult> OverlapResults;

	if (GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeSphere(AttackRange)))
	{
		for (auto result : OverlapResults)
		{
			AURRCharacterMonster* Monster = Cast<AURRCharacterMonster>(result.GetActor());
			if (!Monster) continue;

			UAbilitySystemComponent* TargetASC = Monster->GetAbilitySystemComponent();
			if (!TargetASC) continue;

			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(AttackEffect, 0, Context);
			if (Spec.IsValid())
			{
				URR_LOG(LogURR, Log, TEXT("Overlap: %s"), *Monster->GetName());
				ASC->BP_ApplyGameplayEffectSpecToTarget(Spec, Monster->GetAbilitySystemComponent());
			}
		}

		Destroy();
	}
}

void AURRMine::OnHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//Ground -> ¼³Ä¡
	URR_LOG(LogURR, Log, TEXT("Hit: %s"), *OtherActor->GetName());
	SetActorRotation(FQuat::Identity);
}