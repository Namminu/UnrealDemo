// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdTestGameMode.h"
#include "ThirdTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdTestGameMode::AThirdTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
