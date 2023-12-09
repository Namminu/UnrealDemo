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
	//��Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultComponent;

	//����ƽ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoor;
	
	//Ÿ�� ���� �޾ƿ��� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	int tileCount;

	//Tile�� myNum�� ���� �ٸ� Ÿ�ϵ��� �� Ŭ������ ������ �޾ƿ��� ���� ������
	bool tileNo1;
	bool tileNo2;
	bool tileNo3;
	bool tileNo4;
	
	//tileNum ����Ʈ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	TArray<int32> numList;
	//tile_isTurn bool���� ����Ʈ
	TArray<bool> boolList;

	int indexCount;

public:	
	// Sets default values for this actor's properties
	ABaseOpenDoor();

	//������ Tile���� �Ҵ��ϱ� ���� �Լ�, BP�� ����� �ʿ�� ���ٰ� �Ǵ��� UFUNCTION ���x
	void CastFuncTileNumToVariable(int tileNum);

	//��ġ�� Door���� IsTurn �� �޾ƿ��� �Լ�
	UFUNCTION(BlueprintCallable, Category = "TileEvent")
	bool GetTileIsTurn();

	//Door�� ������ ���� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TileEvent")
	void DoorMoving();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
