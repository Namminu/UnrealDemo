// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseOpenDoor.generated.h"

UCLASS()
class UNREALGISUL_API ABaseOpenDoor : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoor;
	
	//Ÿ�� ���� �޾ƿ��� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	int tileCount;

public:	
	// Sets default values for this actor's properties
	ABaseOpenDoor();

	//��ġ�� Door���� IsTurn �� �޾ƿ��� �Լ�
	UFUNCTION(BlueprintCallable, Category = "TileEvent")
	void GetTileIsTurn();

	//Door�� ������ ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "TileEvent") //���� BlueprintImplementableEvent ���ɼ� ����
	void DoorMoving();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
