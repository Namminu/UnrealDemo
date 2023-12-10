// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBaseObject.h"
#include "Components/BoxComponent.h"
#include "MagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AMoveBaseObject::AMoveBaseObject()
{
	isMagicHit = false;
}

void AMoveBaseObject::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMoveBaseObject::OnOverlapBegin);
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void AMoveBaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isMagicHit)
	{
		MovingGrab(DeltaTime);
	}

}

void AMoveBaseObject::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("222"));
		isMagicHit = true;
	}
}

void AMoveBaseObject::MovingGrab(float DeltaTime)
{
	// 플레이어의 위치 가져오기
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		// 이동 시작 위치와 목표 위치 설정
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = PlayerLocation + (PlayerCharacter->GetActorForwardVector() * 200);

		// 이동 속도와 이동 시간 설정
		float MoveSpeed = 80.0f;  
		float MoveDuration = FVector::Dist(StartLocation, TargetLocation) / MoveSpeed;



		// 경과 시간 증가
		ElapsedTime += DeltaTime;

		// 시간에 따른 보간 계산
		float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

		// 오브젝트 위치 업데이트
		SetActorLocation(NewLocation);

		// 이동이 완료되면 이동 중지
		if (ElapsedTime >= MoveDuration)
		{
			isMagicHit = false;
			ElapsedTime = 0;
		}
	}
}

