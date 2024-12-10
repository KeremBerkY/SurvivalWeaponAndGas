// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "UObject/Object.h"
#include "SingleShotFireMode.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USingleShotFireMode : public UObject, public IFireMode
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* Weapon) { CurrentWeapon = Weapon; }

	
	virtual void StartFire(AWeaponBase* Weapon) override;
	virtual void StopFire(AWeaponBase* Weapon) override;
	virtual FString GetModeName() override { return "Single Shot"; }
	virtual float GetFireRate() const override { return 0.5f; } // Örnek fire rate

	void SingleShotFire();
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
	
};
