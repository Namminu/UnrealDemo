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

	//index�ʱ�ȭ
	indexCount = 0;

}

// Called when the game starts or when spawned
void ABaseOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	//boolList �ʱ�ȭ
	boolList.Init(false, tileCount);

	// Ÿ�� ���� ���
	UE_LOG(LogTemp, Warning, TEXT("Tile Obj: %d"), tileCount);

	//CastFuncTileNumToVariable();
}

// Called every frame
void ABaseOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ǽð����� Ÿ���� bool ������ Ȯ��
	if (GetTileIsTurn())
	{
		DoorMoving();
	}
}

//Ÿ�ϵ��� isTurn ���� �޾ƿ��� ���� �Լ� - BaseTile���� ȣ��
void ABaseOpenDoor::CastFuncTileNumToVariable(int tileNum)
{
	/*TSubclassOf<ABaseTile> baseTileClass = ABaseTile::StaticClass();
	TArray<AActor*> TileArrayInLevel;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), baseTileClass, TileArrayInLevel);

	for (AActor* actor : TileArrayInLevel)
	{
		ABaseTile* theTile = Cast<ABaseTile>(actor);
		if (theTile)
		{
			if (theTile->GetmyNum() == 1)
			{
				tileNo1 = theTile->GetisTurn();
				UE_LOG(LogTemp, Warning, TEXT("No1 Casting OK"));
				UE_LOG(LogTemp, Warning, TEXT("No1 bool : %s"), tileNo1 ? TEXT("True") : TEXT("False"));
			}
			else if (theTile->GetmyNum() == 2)
			{
				tileNo2 = theTile->GetisTurn();
				UE_LOG(LogTemp, Warning, TEXT("No2 Casting OK"));
				UE_LOG(LogTemp, Warning, TEXT("No2 bool : %s"), tileNo2);
			}
			else if (theTile->GetmyNum() == 3)
			{
				tileNo3 = theTile->GetisTurn();
				UE_LOG(LogTemp, Warning, TEXT("No3 Casting OK"));
				UE_LOG(LogTemp, Warning, TEXT("No3 bool : %s"), tileNo3);
			}
			else if (theTile->GetmyNum() == 4)
			{
				tileNo4 = theTile->GetisTurn();
				UE_LOG(LogTemp, Warning, TEXT("No4 Casting OK"));
				UE_LOG(LogTemp, Warning, TEXT("No4 bool : %s"), tileNo4);
			}
		}		
	}*/

	//�Ű������� �Ѿ���� myNum ���� ����Ʈ�� �ε������� ��
	if (tileNum == numList[indexCount])
	{
		//��ġ�� ��� OpenDoor ������Ʈ�� bool ����Ʈ true �� ��ȯ
		boolList[indexCount++] = true;
	}
}

//��� bool ����Ʈ�� �� Ȯ�� - ���� true �̸� true ��ȯ
bool ABaseOpenDoor::GetTileIsTurn()
{
	////������� bool �������� Ȯ���ϱ� ���� ���� if��
	//if (boolList[0] && !boolList[1] && !boolList[2] && !boolList[3])
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No1 Check OK"));
	//	if (boolList[1] && !boolList[2] && !boolList[3])
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("No2 Check OK"));
	//		if (boolList[2] && !boolList[3])
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("No3 Check OK"));
	//			if (boolList[3])
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("No4 Check OK"));
	//				//���� ���� �� �ִϸ��̼� �Լ� ȣ��
	//				DoorMoving();
	//				UE_LOG(LogTemp, Warning, TEXT("Call Moving Animation"));
	//			}
	//		}
	//	}
	//}

	//
	for (int i = 0; i < boolList.Num(); i++)
	{
		if (boolList[i] == false) return false;
	}
	indexCount = 0;
	return true;
}
