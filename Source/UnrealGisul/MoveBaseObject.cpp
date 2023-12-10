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
	// �÷��̾��� ��ġ ��������
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		// �̵� ���� ��ġ�� ��ǥ ��ġ ����
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = PlayerLocation + (PlayerCharacter->GetActorForwardVector() * 200);

		// �̵� �ӵ��� �̵� �ð� ����
		float MoveSpeed = 80.0f;  
		float MoveDuration = FVector::Dist(StartLocation, TargetLocation) / MoveSpeed;



		// ��� �ð� ����
		ElapsedTime += DeltaTime;

		// �ð��� ���� ���� ���
		float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

		// ������Ʈ ��ġ ������Ʈ
		SetActorLocation(NewLocation);

		// �̵��� �Ϸ�Ǹ� �̵� ����
		if (ElapsedTime >= MoveDuration)
		{
			isMagicHit = false;
			ElapsedTime = 0;
		}
	}
}

