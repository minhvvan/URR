// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "URRMonsterEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class URR_API UURRMonsterEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ImgMonster;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMonster;
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void SetMonsterInfo(const struct FMonsterInfo& monsters);

protected:
	UPROPERTY(EditAnywhere)
	TMap<int, UTexture2D*> MonsterImages;
};
