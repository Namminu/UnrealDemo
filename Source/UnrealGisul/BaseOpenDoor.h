// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseOpenDoor.generated.h"

UCLASS()
class UNREALGISUL_API ABaseOpenDoor : public AActor
{
	GENERATED_BODY()
private:
	//루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultComponent;

	//스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoor;
	
	//타일 갯수 받아오는 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	int tileCount;

	//Tile의 myNum에 따라 다른 타일들을 이 클래스의 변수로 받아오기 위한 변수들
	bool tileNo1;
	bool tileNo2;
	bool tileNo3;
	bool tileNo4;
	
	//tileNum 리스트
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	TArray<int32> numList;
	//tile_isTurn bool변수 리스트
	TArray<bool> boolList;

	int indexCount;

public:	
	// Sets default values for this actor's properties
	ABaseOpenDoor();

	//레벨의 Tile들을 할당하기 위한 함수, BP에 노출될 필요는 없다고 판단해 UFUNCTION 사용x
	void CastFuncTileNumToVariable(int tileNum);

	//배치된 Door들의 IsTurn 값 받아오는 함수
	UFUNCTION(BlueprintCallable, Category = "TileEvent")
	bool GetTileIsTurn();

	//Door가 열리기 위한 함수
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TileEvent")
	void DoorMoving();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
