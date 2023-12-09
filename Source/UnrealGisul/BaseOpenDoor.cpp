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
	//왼쪽 벽 Component
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

	//오른쪽 벽 Component 
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RIGHTDoor"));
	RightDoor->AttachToComponent(DefaultComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh 와 Meterial 은 이미 불러온 에셋을 사용하므로 변수 그대로 사용
	RightDoor->SetStaticMesh(CubeMeshAsset.Object);
	RightDoor->SetMaterial(0, MyMaterialAsset.Object);
	RightDoor->SetRelativeLocation(FVector(0.0f, 110.f, 0.0f));
	RightDoor->SetWorldScale3D(FVector(0.2f, 2.25f, 3.25f));

	//타일 갯수 받아와서 그 값을 tileCount에 할당
	//왜인지 Door오브젝트를 레벨에 배치하는 순간에만 값이 판단됨
	UClass* TileObjClass = ABaseTile::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TileObjClass, FoundActors);
	tileCount = FoundActors.Num();

	//index초기화
	indexCount = 0;

}

// Called when the game starts or when spawned
void ABaseOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	//boolList 초기화
	boolList.Init(false, tileCount);

	// 타일 갯수 출력
	UE_LOG(LogTemp, Warning, TEXT("Tile Obj: %d"), tileCount);

	//CastFuncTileNumToVariable();
}

// Called every frame
void ABaseOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//실시간으로 타일의 bool 변수값 확인
	if (GetTileIsTurn())
	{
		DoorMoving();
	}
}

//타일들의 isTurn 값을 받아오기 위한 함수 - BaseTile에서 호출
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

	//매개변수로 넘어오는 myNum 값과 리스트의 인덱스값을 비교
	if (tileNum == numList[indexCount])
	{
		//일치할 경우 OpenDoor 오브젝트의 bool 리스트 true 로 전환
		boolList[indexCount++] = true;
	}
}

//모든 bool 리스트의 값 확인 - 전부 true 이면 true 반환
bool ABaseOpenDoor::GetTileIsTurn()
{
	////순서대로 bool 변수값을 확인하기 위한 다중 if문
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
	//				//조건 만족 시 애니메이션 함수 호출
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
