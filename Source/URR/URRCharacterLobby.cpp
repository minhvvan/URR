// Fill out your copyright notice in the Description page of Project Settings.


#include "URRCharacterLobby.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/URRLobbyPC.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "URR.h"

AURRCharacterLobby::AURRCharacterLobby() :
	bShouldMove(false),
	CachedDestination(FVector::ZeroVector),
	FollowTime(0.f)
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
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1400.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetLeaderPoseComponent(GetMesh());

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetLeaderPoseComponent(GetMesh());

	AccMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AccMesh"));
	AccMesh->SetupAttachment(RootComponent);
	AccMesh->SetLeaderPoseComponent(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_Helmet.SK_Helmet'"));
	if (HeadMeshRef.Succeeded())
	{
		HeadMesh->SetSkeletalMesh(HeadMeshRef.Object);
		HeadMesh->SetRelativeLocation(FVector(0, 0, -90));
		HeadMesh->SetRelativeRotation(FRotator(0, -90, 0));
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_BodyKit.SK_BodyKit'"));
	if (BodyMeshRef.Succeeded())
	{
		BodyMesh->SetSkeletalMesh(BodyMeshRef.Object);
		BodyMesh->SetRelativeLocation(FVector(0, 0, -90));
		BodyMesh->SetRelativeRotation(FRotator(0, -90, 0));
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> AccMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/soldier_battle_pack/Mesh/Soldier/SK_BackPack.SK_BackPack'"));
	if (AccMeshRef.Succeeded())
	{
		AccMesh->SetSkeletalMesh(AccMeshRef.Object);
		AccMesh->SetRelativeLocation(FVector(0, 0, -90));
		AccMesh->SetRelativeRotation(FRotator(0, -90, 0));
	}
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

	URR_LOG(LogURR, Log, TEXT("%s"), *WorldDirection.ToString());
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