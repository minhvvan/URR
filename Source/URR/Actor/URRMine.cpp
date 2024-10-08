// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRMine.h"
#include "Components/SphereComponent.h"
#include "Character/URRCharacterMonster.h"
#include "Attribute/URRProjectileAttributeSet.h"
#include "Physics/URRCollision.h"
#include "Tag/URRGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "URR.h"

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
}

void AURRMine::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || OtherActor == GetOwner()) return;

	TArray<FOverlapResult> OverlapResults;
	float ExplosionRange = ProjectileAttributeSet->GetExplosionRange();

	if (GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, CCHANNEL_URRATTACK, FCollisionShape::MakeSphere(ExplosionRange)))
	{
		for (auto result : OverlapResults)
		{
			AURRCharacterMonster* Monster = Cast<AURRCharacterMonster>(result.GetActor());
			if (!Monster) continue;

			UAbilitySystemComponent* TargetASC = Monster->GetAbilitySystemComponent();
			if (!TargetASC) continue;

			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle AttackSpec = ASC->MakeOutgoingSpec(AttackEffect, 0, Context);
			if (AttackSpec.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(AttackSpec, Monster->GetAbilitySystemComponent());
			}

			FGameplayEffectSpecHandle DebuffSpec = ASC->MakeOutgoingSpec(DebuffEffect, 0, Context);
			if (DebuffSpec.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(DebuffSpec, Monster->GetAbilitySystemComponent());
			}
		}

		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSFX);
		InvokeGC();
		Destroy();
	}
}

void AURRMine::OnHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	SetActorRotation(FQuat::Identity);
}