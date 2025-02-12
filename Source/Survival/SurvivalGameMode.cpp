// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameMode.h"
#include "SurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"

ASurvivalGameMode::ASurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")); // TODO: Karakter blueprintini değiştir!
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// HUD sınıfını ASurvivalSystemHUD olarak ayarlama
	HUDClass = ASurvivalSystemHUD::StaticClass();
}

UCharacterClassInfo* ASurvivalGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}
