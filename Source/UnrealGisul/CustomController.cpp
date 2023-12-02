// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomController.h"
#include <Engine/Classes/Kismet/KismetMathLibrary.h>

ACustomController::ACustomController()
{
	bShowMouseCursor = true;
}

void ACustomController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	LookMouseCursor();
}

void ACustomController::LookMouseCursor()
{

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		APawn* const MyPawn = GetPawn();
		if (MyPawn)

		{
			FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(),
				FVector(Hit.Location.X, Hit.Location.Y, MyPawn->GetActorLocation().Z));
			MyPawn->SetActorRotation(LookRotation);
		}

	}
}