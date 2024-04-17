// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/URRCharacterMonster.h"
#include "AbilitySystemComponent.h"
#include "Attribute/MonsterAttributeSet.h"
#include "URR.h"

AURRCharacterMonster::AURRCharacterMonster()
{
    MonsterAttributeSet = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("MonsterAttributeSet"));

}

UAbilitySystemComponent* AURRCharacterMonster::GetAbilitySystemComponent() const
{
    return ASC;
}

void AURRCharacterMonster::BeginPlay()
{
    Super::BeginPlay();
}

void AURRCharacterMonster::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void AURRCharacterMonster::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

}
