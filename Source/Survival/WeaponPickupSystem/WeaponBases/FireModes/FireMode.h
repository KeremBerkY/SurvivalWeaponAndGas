// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FireMode.generated.h"

class AWeaponBase;
// This class does not need to be modified.
UINTERFACE()
class UFireMode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVAL_API IFireMode
{
	GENERATED_BODY()

public:
	
	UFUNCTION(Category = "Fire Mode")
	virtual void StartFire(AWeaponBase* Weapon) = 0;
	
	UFUNCTION(Category = "Fire Mode")
	virtual void StopFire(AWeaponBase* Weapon) = 0;
	
	UFUNCTION(Category = "Fire Mode")
	virtual FString GetModeName() = 0;
	
	UFUNCTION(Category = "Fire Mode")
	virtual float GetFireRate() const = 0;

	// // Ateşleme modunu bağlamak için gerekli fonksiyon
	// UFUNCTION(Category = "Fire Mode")
	// virtual void InitializeMode(AWeaponBase* InWeapon) = 0;

};
