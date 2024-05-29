// Fill out your copyright notice in the Description page of Project Settings.


#include "URRCharacterLobby.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Framework/URRLobbyPC.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "URR.h"

AURRCharacterLobby::AURRCharacterLobby()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_Soldier.SK_Soldier'"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2400.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AURRCharacterLobby::BeginPlay()
{
	Super::BeginPlay();

	if (!PC) PC = Cast<AURRLobbyPC>(Controller);
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AURRCharacterLobby::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AURRCharacterLobby::OnInputStarted);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AURRCharacterLobby::OnSetDestinationTriggered);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AURRCharacterLobby::OnSetDestinationReleased);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Canceled, this, &AURRCharacterLobby::OnSetDestinationReleased);
}

void AURRCharacterLobby::OnInputStarted()
{
	if (!PC) PC = Cast<AURRLobbyPC>(Controller);

	PC->StopMovement();
}

void AURRCharacterLobby::OnSetDestinationTriggered()
{
	if (!PC) PC = Cast<AURRLobbyPC>(Controller);

	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(WorldDirection, 1.0, false);
}

void AURRCharacterLobby::OnSetDestinationReleased()
{
	if (!PC) PC = Cast<AURRLobbyPC>(Controller);
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC, CachedDestination);

		if (FXCursor)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(PC, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}

	FollowTime = 0.f;
}