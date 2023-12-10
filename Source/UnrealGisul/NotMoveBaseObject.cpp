// Fill out your copyright notice in the Description page of Project Settings.


#include "NotMoveBaseObject.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "MagicProjectile.h"
#include "Kismet/GameplayStatics.h"


ANotMoveBaseObject::ANotMoveBaseObject()
{
	isMagicHit = false;
}

void ANotMoveBaseObject::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANotMoveBaseObject::OnOverlapBegin);
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void ANotMoveBaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isMagicHit)
	{
		MovingGrab(DeltaTime);
	}
}

void ANotMoveBaseObject::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();
		isMagicHit = true;

		//�浹 ���� ��������
		CollisionDirection = SweepResult.ImpactNormal;
	}
}

void ANotMoveBaseObject::MovingGrab(float DeltaTime)
{
	// �÷��̾��� ��ġ ��������
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		// �̵� ���� ��ġ�� ��ǥ ��ġ ����
		FVector StartLocation = PlayerLocation;
		FVector TargetLocation = GetActorLocation() - (CollisionDirection * 200);

		// �̵� �ӵ��� �̵� �ð� ����
		float MoveSpeed = 80.0f;
		float MoveDuration = FVector::Dist(StartLocation, TargetLocation) / MoveSpeed;

		// ��� �ð� ����
		ElapsedTime += DeltaTime;

		// �ð��� ���� ���� ���
		float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

		// ������Ʈ ��ġ ������Ʈ
		PlayerCharacter->SetActorLocation(NewLocation);

		// �̵��� �Ϸ�Ǹ� �̵� ����
		if (ElapsedTime >= MoveDuration)
		{
			isMagicHit = false;
			ElapsedTime = 0;
		}
	}
}