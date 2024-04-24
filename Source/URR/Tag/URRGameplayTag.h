
#pragma once

#include "GameplayTagContainer.h"

#define URRTAG_PLAYER_CANSPAWN FGameplayTag::RequestGameplayTag(FName("Player.State.CanSpawn"))
#define URRTAG_UNIT_ATTACK FGameplayTag::RequestGameplayTag(FName("Unit.Action.Attack"))
#define URRTAG_MONSTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Monster.State.IsDead"))

