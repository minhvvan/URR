// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRProjectile.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AURRProjectile::AURRProjectile()
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

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AURRProjectile::BeginOverlapCallback);
}

void AURRProjectile::BeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}