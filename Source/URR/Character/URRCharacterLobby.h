// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/URRCharacterBase.h"
#include "URRCharacterLobby.generated.h"

/**
 * 
 */
UCLASS()
class URR_API AURRCharacterLobby : public AURRCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> AccMesh;


public:
	AURRCharacterLobby();

	void SetReady(bool ready);

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomInAction;

	UFUNCTION()
	void OnInputStarted();

	UFUNCTION()
	void OnSetDestinationTriggered();

	UFUNCTION()
	void OnSetDestinationReleased();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY()
	TObjectPtr<class AURRLobbyPC> PC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UNiagaraSystem> FXCursor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bReady;

private:
	FVector CachedDestination;
	float FollowTime; 
};
