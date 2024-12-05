// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireMode.h"
#include "UObject/Object.h"
#include "BurstShotFireMode.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UBurstShotFireMode : public UObject, public IFireMode
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* Weapon) { CurrentWeapon = Weapon; }

	virtual void StartFire(AWeaponBase* Weapon) override;
	virtual void StopFire(AWeaponBase* Weapon) override;
	virtual FString GetModeName() override { return "Burst Fire"; }
	virtual float GetFireRate() const override { return 0.5f; } // Örnek fire rate
	
	void FireShot();
	void StartBurstTimer();
	void BurstFireShot();
	void FinishedBurstCountTimer();
	void ResetBurstCount();

private:
	UPROPERTY()
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Weapon | BurstCountCooldown")
	float BurstCountCooldown = 0.2f;

	FTimerHandle BurstShotTimerHandle;
	
};
