// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "BaseOpenDoor.h"

// Sets default values
ABaseTile::ABaseTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	Body->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		Body->SetStaticMesh(CubeMeshAsset.Object);
	}
	//��ü���� ���׸��� ����
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MyMaterialAsset(TEXT("/Game/Content/Minwoo_Develop/Object/YellowMT"));
	if (MyMaterialAsset.Succeeded())
	{
		Body->SetMaterial(0, MyMaterialAsset.Object);
	}
	Body->SetWorldScale3D(FVector(2.0f, 2.0f, 0.1f));

	//Box Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetWorldScale3D(FVector(1.0f, 1.0f, 2.0f));

	//Particle
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("/Game/StarterContent/Particles/P_Sparks"));
	if (ParticleSystemAsset.Succeeded())
	{
		ParticleComponent->SetTemplate(ParticleSystemAsset.Object);
		//ParticleComponent->ActivateSystem();
	}
	ParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));
	
	delayTime = 5.0;
}

// Called when the game starts or when spawned
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();

	//Box Collision OverlapEvent Bind
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseTile::BeginOverlap);

	//���� �ʱ�ȭ
	isTurn = false;
}

// Called every frame
void ABaseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectOnOff_Implementation();
}

//�������� ���� ����Ʈ on/off �Լ�
void ABaseTile::EffectOnOff_Implementation()
{
	if (isTurn == true)
	{
		ParticleComponent->SetVisibility(true);
		ParticleComponent->SetComponentTickEnabled(true);

		//delayTime ��ŭ ������ ��
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DelayBool"));
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, delayTime, false);
	}
	else if (isTurn == false)
	{
		ParticleComponent->SetVisibility(false);
		ParticleComponent->SetComponentTickEnabled(false);
	}
}

//�ڽ� �ݸ��� ������ �̺�Ʈ
void ABaseTile::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�±� �߰� - �÷��̾� �Ǻ�
	if (OtherComp->ComponentHasTag("PLAYER"))
	{
		isTurn = true;

		UE_LOG(LogTemp, Warning, TEXT("Overlap"));

		CallOpenDoorCast();
	} 
}

//������ �� �Ͼ�� �̺�Ʈ
void ABaseTile::DelayBool()
{
	isTurn = false;
}

void ABaseTile::CallOpenDoorCast()
{
	ABaseOpenDoor* openDoor = Cast<ABaseOpenDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseOpenDoor::StaticClass()));

	openDoor->CastFuncTileNumToVariable(myNum);
}