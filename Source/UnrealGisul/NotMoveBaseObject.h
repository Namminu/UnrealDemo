// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObject.h"
#include "NotMoveBaseObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGISUL_API ANotMoveBaseObject : public ABaseObject
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	// Called every frame
	ANotMoveBaseObject();
	virtual void Tick(float DeltaTime) override;
	void MovingGrab(float DeltaTime);

private:
	// 충돌 방향 가져오기
	FVector CollisionDirection;
};
