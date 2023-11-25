// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGisulGameMode.h"
#include "UnrealGisulCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealGisulGameMode::AUnrealGisulGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/WizardSM_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
