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
	//��ü Ÿ���� ������ ���� ����
	UPROPERTY(EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	int myNum;

	//Ÿ���� �÷��̾�� �������� üũ�ϱ� ���� bool ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	bool isTurn;

	//����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;

	//isTUrn �� true�� ���� ������ ���� ��ƼŬ ������Ʈ
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;

	//�÷��̾���� �������� �˻��� �ݸ��� �ڽ�
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	//������ �ð�
	UPROPERTY(EditAnywhere, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	float delayTime;

public:
	// Sets default values for this actor's properties
	ABaseTile();

	//��ƼŬ ����Ʈ�� �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Tile Count", meta = (AllowPrivateAccess = "true"))
	void EffectOnOff();

	//������ �̺�Ʈ
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect Event", meta = (AllowPrivateAccess = "true"))
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//������ �Լ� - isTurn �� ���� �ٲٱ� ����
	UFUNCTION()
	void DelayBool();

	//OpenDoor ������Ʈ�� ĳ���� �Լ� ȣ�� Ÿ�̹��� ���� �Լ�
	void CallOpenDoorCast();

	//myNum�� ��ȯ�ϱ� ���� �Լ�
	const int GetmyNum() { return myNum; }
	//isTurn�� ��ȯ�ϱ� ���� �Լ�
	const bool GetisTurn() { return isTurn; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
