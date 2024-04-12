// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterBase.h"
#include "AbilitySystemComponent.h"

// Sets default values
AURRCharacterBase::AURRCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AURRCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AURRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//ASC->InitAbilityActorInfo(this, this);
}

// Called every frame
void AURRCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AURRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

