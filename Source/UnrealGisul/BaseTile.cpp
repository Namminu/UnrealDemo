// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

// Sets default values
ABaseTile::ABaseTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	Body->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Box Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	//Box Collision OverlapEvent Bind
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseTile::BeginOverlap);

	//Particle
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("/Game/StarterContent/Particles/P_Sparks"));
	if (ParticleSystemAsset.Succeeded())
	{
		ParticleComponent->SetTemplate(ParticleSystemAsset.Object);
		//ParticleComponent->ActivateSystem();
	}

	//���� �ʱ�ȭ
	isTurn = false;
	delayTime = 5.0;
}

// Called when the game starts or when spawned
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();

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
	isTurn = true;

	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
}

//������ �� �Ͼ�� �̺�Ʈ
void ABaseTile::DelayBool()
{
	isTurn = false;
}