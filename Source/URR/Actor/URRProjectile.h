// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "URRProjectile.generated.h"

UCLASS()
class URR_API AURRProjectile : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AURRProjectile();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> Capsule;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UURRProjectileAttributeSet> ProjectileAttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> AttackEffect;

	//UPROPERTY(EditAnywhere, Category = GAS)
	//TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> DebuffEffect;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> ExplosionSFX;

protected:
	//UPROPERTY(EditAnywhere, Category = STAT)
	//float AttackRate;

	UPROPERTY(EditAnywhere, Category = STAT)
	float ExplosionRange;

	//UPROPERTY(EditAnywhere, Category = STAT)
	//float KnockBackDist;

	//UPROPERTY(EditAnywhere, Category = STAT)
	//float SlowRate;

	//UPROPERTY(EditAnywhere, Category = STAT)
	//float CriticalProb;

	//UPROPERTY(EditAnywhere, Category = STAT)
	//float CriticalRate;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GCTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(FVector vel);

protected:
	UFUNCTION()
	virtual void BeginOverlapCallback(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};

	UFUNCTION()
	virtual void OnHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {};

	void InvokeGC();
};
