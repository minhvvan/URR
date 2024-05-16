// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "URRGameInstance.generated.h"


UENUM(BlueprintType)
enum class EUnitType : uint8 
{ 
	UT_Pistol UMETA(DisplayName = "Pistol"), 
	UT_Rifle UMETA(DisplayName = "Rifle"),
	UT_Mine UMETA(DisplayName = "Mine"),
	UT_Rocket UMETA(DisplayName = "Rocket"),
	UT_MechrPistol UMETA(DisplayName = "MechaPistol"),
	UT_MechaRifle UMETA(DisplayName = "MechaRifle"),
	UT_MechaShotGun UMETA(DisplayName = "MechaShotGun"),
	UT_MechaSniper UMETA(DisplayName = "MechaSniper"),
};

UENUM(BlueprintType)
enum class EFindTargetType : uint8
{
	FT_First UMETA(DisplayName = "First"),
	FT_Last UMETA(DisplayName = "Last"),
	FT_MaxHP UMETA(DisplayName = "MaxHP"),
	FT_MinHP UMETA(DisplayName = "MinHP"),
};

USTRUCT()
struct FFindTargetText : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Text;
};

UCLASS()
class URR_API UURRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UURRGameInstance();

protected:

};
