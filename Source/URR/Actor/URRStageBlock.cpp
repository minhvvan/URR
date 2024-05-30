// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/URRStageBlock.h"
#include "Physics/URRCollision.h"
#include "Components/SphereComponent.h"
#include "UI/URRStageWidget.h"
#include "URRCharacterLobby.h"

// Sets default values
AURRStageBlock::AURRStageBlock()
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
		StageWidget->AddToViewport();
	}

	//TODO: Block Material 변경(Anim쓰자)
}

void AURRStageBlock::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AURRCharacterLobby* Character = Cast<AURRCharacterLobby>(OtherActor);
	if (!Character) return;

	Character->SetReady(false);

	if (StageWidget) StageWidget->DetachWidget();

	//TODO: Block Material 변경(Anim쓰자)
}

