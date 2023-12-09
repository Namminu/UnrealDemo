// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBaseObject.h"
#include "Components/BoxComponent.h"
#include "MagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"



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

}

void AMoveBaseObject::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("222"));
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (PlayerCharacter)
		{
			// 플레이어의 위치 가져오기
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();

			// 오브젝트의 위치를 플레이어 앞으로 이동시키기
			FVector NewLocation = PlayerLocation + (PlayerCharacter->GetActorForwardVector() * 200);

			// 오브젝트 위치 설정
			SetActorLocation(NewLocation);
		}
	}
}