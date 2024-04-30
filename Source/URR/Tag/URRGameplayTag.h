
#pragma once

#include "GameplayTagContainer.h"

#define URRTAG_PLAYER_CANSPAWN FGameplayTag::RequestGameplayTag(FName("Player.State.CanSpawn"))
#define URRTAG_UNIT_ATTACK FGameplayTag::RequestGameplayTag(FName("Unit.Action.Attack"))
#define URRTAG_UNIT_ATTACKING FGameplayTag::RequestGameplayTag(FName("Unit.State.IsAttacking"))
#define URRTAG_MONSTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Monster.State.IsDead"))
#define URRTAG_DATA_ATTACKRANGE FGameplayTag::RequestGameplayTag(FName("Data.AttackRange"))
#define URRTAG_DATA_ATTACKRATE FGameplayTag::RequestGameplayTag(FName("Data.AttackRate"))
#define URRTAG_DATA_KNOCKBACK FGameplayTag::RequestGameplayTag(FName("Data.KnockBackDistance"))
#define URRTAG_DATA_SLOW FGameplayTag::RequestGameplayTag(FName("Data.SlowRate"))