// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRProjectile.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Attribute/URRProjectileAttributeSet.h"
#include "Tag/URRGameplayTag.h"


// Sets default values
AURRProjectile::AURRProjectile(): 
	KnockBackDist(0),
	SlowRate(0),
	ExplosionRange(500.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	Capsule = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule"));
	Capsule->InitSphereRadius(15.0f);
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(Capsule);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	ProjectileAttributeSet = CreateDefaultSubobject<UURRProjectileAttributeSet>(TEXT("ProjectileAttributeSet"));
}

UAbilitySystemComponent* AURRProjectile::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AURRProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AURRProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AURRProjectile::FireInDirection(FVector vel)
{
	ProjectileMovementComponent->Velocity = vel;
}

void AURRProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ASC->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InitStatEffect, 0, Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(URRTAG_DATA_ATTACKRANGE, ExplosionRange);
		Spec.Data->SetSetByCallerMagnitude(URRTAG_DATA_ATTACKRATE, AttackRate);
		Spec.Data->SetSetByCallerMagnitude(URRTAG_DATA_KNOCKBACK, KnockBackDist);
		Spec.Data->SetSetByCallerMagnitude(URRTAG_DATA_SLOW, SlowRate);
		ASC->BP_ApplyGameplayEffectSpecToSelf(Spec);
	}

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AURRProjectile::BeginOverlapCallback);
	Capsule->OnComponentHit.AddDynamic(this, &AURRProjectile::OnHitCallback);
}