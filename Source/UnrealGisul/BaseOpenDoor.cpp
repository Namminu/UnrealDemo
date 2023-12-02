// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseOpenDoor.h"

// Sets default values
ABaseOpenDoor::ABaseOpenDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseOpenDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

