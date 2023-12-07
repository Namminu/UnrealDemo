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
	
	//타일 갯수 받아오는 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TileEvent", meta = (AllowPrivateAccess = "true"))
	int tileCount;

public:	
	// Sets default values for this actor's properties
	ABaseOpenDoor();

	//배치된 Door들의 IsTurn 값 받아오는 함수
	UFUNCTION(BlueprintCallable, Category = "TileEvent")
	void GetTileIsTurn();

	//Door가 열리기 위한 함수
	UFUNCTION(BlueprintCallable, Category = "TileEvent") //추후 BlueprintImplementableEvent 가능성 존재
	void DoorMoving();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
