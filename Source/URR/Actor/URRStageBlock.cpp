// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRStageBlock.h"
#include "Physics/URRCollision.h"
#include "Components/SphereComponent.h"
#include "UI/URRStageWidget.h"
#include "Character/URRCharacterLobby.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/URRStageSG.h"
#include "URR.h"

// Sets default values
AURRStageBlock::AURRStageBlock():
	stageNum(0),
	stageScore(0),
	bComplete(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetSphereRadius(160.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/URR/Mesh/StageBlock.StageBlock'"));
	if (MeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}	
	
	ConstructorHelpers::FClassFinder<UURRStageWidget> StageWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/URR/UI/WBP_StageWidget.WBP_StageWidget_C'"));
	if (StageWidgetRef.Succeeded())
	{
		StageWidgetClass = StageWidgetRef.Class;
	}
}

// Called when the game starts or when spawned
void AURRStageBlock::BeginPlay()
{
	Super::BeginPlay();
	
	UURRStageSG* LoadGameInstance = Cast<UURRStageSG>(UGameplayStatics::CreateSaveGameObject(UURRStageSG::StaticClass()));
	
	FString SlotName(TEXT("Stage"));
	SlotName += FString::Printf(TEXT("%d"), stageNum);
	LoadGameInstance = Cast<UURRStageSG>(UGameplayStatics::LoadGameFromSlot(SlotName, LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		stageScore = LoadGameInstance->Score;
		bComplete = true;

		if (!BlockMats[EBlockMat::CLEAR]) return;
		Mesh->SetMaterial(0, BlockMats[EBlockMat::CLEAR]);
	}
	else
	{
		UURRStageSG* PrevLoadInstance = Cast<UURRStageSG>(UGameplayStatics::CreateSaveGameObject(UURRStageSG::StaticClass()));

		//이전 단계 확인
		FString PrevStageName(TEXT("Stage"));
		PrevStageName += FString::Printf(TEXT("%d"), stageNum - 1);
		PrevLoadInstance = Cast<UURRStageSG>(UGameplayStatics::LoadGameFromSlot(PrevStageName, PrevLoadInstance->UserIndex));

		if (!PrevLoadInstance) return;
		if (!BlockMats[EBlockMat::CURRENT]) return;
		Mesh->SetMaterial(0, BlockMats[EBlockMat::CURRENT]);
	}
}

void AURRStageBlock::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AURRStageBlock::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AURRStageBlock::OnEndOverlap);
}

// Called every frame
void AURRStageBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AURRStageBlock::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AURRCharacterLobby* Character = Cast<AURRCharacterLobby>(OtherActor);
	if (!Character) return;

	Character->SetReady(true);

	if (StageWidgetClass)
	{
		StageWidget = CreateWidget<UURRStageWidget>(GetWorld(), StageWidgetClass);
		StageWidget->SetStageNum(stageNum);
		StageWidget->SetStageScore(stageScore);
		StageWidget->SetStageCompleted(bComplete);
		StageWidget->AddToViewport();
	}
}

void AURRStageBlock::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AURRCharacterLobby* Character = Cast<AURRCharacterLobby>(OtherActor);
	if (!Character) return;

	Character->SetReady(false);

	if (StageWidget) StageWidget->DetachWidget();
}

