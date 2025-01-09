// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_Assignment1GameMode.h"
#include "UE_Assignment1PlayerController.h"
#include "UE_Assignment1Character.h"
#include "UObject/ConstructorHelpers.h"

AUE_Assignment1GameMode::AUE_Assignment1GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUE_Assignment1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}