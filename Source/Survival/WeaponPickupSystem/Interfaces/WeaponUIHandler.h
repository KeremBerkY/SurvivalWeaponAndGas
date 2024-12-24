// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponUIHandler.generated.h"

class ASurvivalCharacter;
class AWeaponBase;
// This class does not need to be modified.
UINTERFACE()
class UWeaponUIHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVAL_API IWeaponUIHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter) = 0;
	virtual void UpdateUI(AWeaponBase* Weapon) = 0;
	virtual void SetupUI(AWeaponBase* Weapon) = 0;
	virtual void RemoveUI(AWeaponBase* Weapon) = 0;
};
