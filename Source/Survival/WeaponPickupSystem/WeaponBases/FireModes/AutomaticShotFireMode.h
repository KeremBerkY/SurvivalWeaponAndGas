// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "UObject/Object.h"
#include "AutomaticShotFireMode.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UAutomaticShotFireMode : public UObject, public IFireMode
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* Weapon) { CurrentWeapon = Weapon; }

	virtual void StartFire(AWeaponBase* Weapon) override;
	virtual void StopFire(AWeaponBase* Weapon) override;
	virtual FString GetModeName() override { return "Automatic"; };
	virtual float GetFireRate() const override { return 0.5f; };
	
	void StartAutomaticTimer();
	void AutomaticFire();

private:
	UPROPERTY()
	AWeaponBase* CurrentWeapon;
	
};
