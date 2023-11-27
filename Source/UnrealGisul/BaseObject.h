// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObject.generated.h"

UCLASS()
class UNREALGISUL_API ABaseObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Body;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MoveAble")
	bool isCanMove = false;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MoveAble")
	void MoveAble();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
