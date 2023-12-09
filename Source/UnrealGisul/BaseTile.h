// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

UCLASS()
class UNREALGISUL_API ABaseTile : public AActor
{
	GENERATED_BODY()

private:
	//전체 타일의 순서를 정할 변수
	UPROPERTY(EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	int myNum;

	//타일이 플레이어에게 밟혔는지 체크하기 위한 bool 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	bool isTurn;

	//스테틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;

	//isTUrn 이 true일 때의 연출을 위한 파티클 컴포넌트
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	//플레이어와의 오버랩을 검사할 콜리전 박스
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	//딜레이 시간
	UPROPERTY(EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	float delayTime;

public:
	// Sets default values for this actor's properties
	ABaseTile();

	//파티클 이펙트를 조절하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	void EffectOnOff();

	//오버랩 이벤트
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect Event", meta = (AllowPrivateAccess = "true"))
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//딜레이 함수 - isTurn 의 값을 바꾸기 위함
	UFUNCTION()
	void DelayBool();

	//OpenDoor 오브젝트의 캐스팅 함수 호출 타이밍을 위한 함수
	void CallOpenDoorCast();

	//myNum을 반환하기 위한 함수
	const int GetmyNum() { return myNum; }
	//isTurn을 반환하기 위한 함수
	const bool GetisTurn() { return isTurn; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
