// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseOpenDoor.h"
#include "Kismet/GameplayStatics.h"
#include "BaseTile.h"

// Sets default values
ABaseOpenDoor::ABaseOpenDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//DefaultRootComponent
	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;
	//���� �� Component
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEFTDoor"));
	LeftDoor->AttachToComponent(DefaultComponent, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		LeftDoor->SetStaticMesh(CubeMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MyMaterialAsset(TEXT("/Game/Content/StarterContent/Materials/M_Brick_Clay_Beveled"));
	if (MyMaterialAsset.Succeeded())
	{
		LeftDoor->SetMaterial(0, MyMaterialAsset.Object);
	}
	LeftDoor->SetRelativeLocation(FVector(0.0f, -110.f, 0.0f));
	LeftDoor->SetWorldScale3D(FVector(0.2f, 2.25f, 3.25f));

	//������ �� Component 
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RIGHTDoor"));
	RightDoor->AttachToComponent(DefaultComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh �� Meterial �� �̹� �ҷ��� ������ ����ϹǷ� ���� �״�� ���
	RightDoor->SetStaticMesh(CubeMeshAsset.Object);
	RightDoor->SetMaterial(0, MyMaterialAsset.Object);
	RightDoor->SetRelativeLocation(FVector(0.0f, 110.f, 0.0f));
	RightDoor->SetWorldScale3D(FVector(0.2f, 2.25f, 3.25f));

	//Ÿ�� ���� �޾ƿͼ� �� ���� tileCount�� �Ҵ�
	//������ Door������Ʈ�� ������ ��ġ�ϴ� �������� ���� �Ǵܵ�
	UClass* TileObjClass = ABaseTile::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TileObjClass, FoundActors);
	tileCount = FoundActors.Num();
}

// Called when the game starts or when spawned
void ABaseOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	// Ÿ�� ���� ���
	UE_LOG(LogTemp, Warning, TEXT("Tile Obj: %d"), tileCount);
}

// Called every frame
void ABaseOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//
void ABaseOpenDoor::GetTileIsTurn()
{

}