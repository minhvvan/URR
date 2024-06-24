// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRRocket.h"
#include "Components/SphereComponent.h"
#include "Character/URRCharacterMonster.h"
#include "Attribute/URRProjectileAttributeSet.h"
#include "Physics/URRCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Tag/URRGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "URR.h"

void AURRRocket::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AURRRocket::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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

		InvokeGC();
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSFX);
		Destroy();
	}
}

void AURRRocket::OnHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
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
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(AttackEffect, 0, Context);
			if (Spec.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(Spec, Monster->GetAbilitySystemComponent());
			}
		}
	}

	InvokeGC();
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSFX);
	Destroy();
}