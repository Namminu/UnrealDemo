// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomController.generated.h"
#include "UnrealGisulGameMode.generated.h"

UCLASS(minimalapi)
class AUnrealGisulGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealGisulGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwitchController")
	TSubclassOf<APlayerController> MyDefaultController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwitchController")
	TSubclassOf<ACustomController> CustomController;
};



