// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/URRGA_AttackSpwanActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/URRCharacterUnit.h"
#include "Character/URRCharacterMonster.h"
#include "Actor/URRProjectile.h"
#include "Kismet/GameplayStatics.h"

UURRGA_AttackSpwanActor::UURRGA_AttackSpwanActor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UURRGA_AttackSpwanActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TriggerEventData->TargetData, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);

		FVector TargetLoc = HitResult.GetActor()->GetActorLocation();
		//actor spawn

		AURRCharacterUnit* Unit = CastChecked<AURRCharacterUnit>(ActorInfo->AvatarActor.Get());
		AURRCharacterMonster* TargetMonster = Unit->GetTargetMonster();
		TSubclassOf<AURRProjectile> Projectileclass = Unit->GetProjectileClass();

		if (Projectileclass && TargetMonster)
		{
			FActorSpawnParameters params;
			FVector SpawnLoc = Unit->GetActorLocation();
			FVector EndLoc = TargetMonster->GetActorLocation();

			AURRProjectile* Projectile = GetWorld()->SpawnActor<AURRProjectile>(Projectileclass, SpawnLoc, FRotator::ZeroRotator, params);
			if (Projectile)
			{
				FVector outVelocity;
				UGameplayStatics::SuggestProjectileVelocity(GetWorld(), OUT outVelocity, SpawnLoc, EndLoc, 10.f, false);
			
				Projectile->FireInDirection(outVelocity);
			}
		}
	}
}