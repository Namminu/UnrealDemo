// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
//#include "Kismet/GameplayStatics.h"
// Sets default values
ABaseObject::ABaseObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	//Body->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!CollisionComponent)
	{
		// �ڽ��� �ܼ� �ݸ��� ǥ������ ����մϴ�.
		CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		// �ڽ��� �ݸ��� �ݰ��� �����մϴ�.
		CollisionComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
		// ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �ǵ��� �����մϴ�.
		RootComponent = CollisionComponent;
	}
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (Mesh.Succeeded())
	{
		Body->SetStaticMesh(Mesh.Object);
	}
	Body->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Body->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	Body->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseObject::BeginPlay()
{
	Super::BeginPlay();
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void ABaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
