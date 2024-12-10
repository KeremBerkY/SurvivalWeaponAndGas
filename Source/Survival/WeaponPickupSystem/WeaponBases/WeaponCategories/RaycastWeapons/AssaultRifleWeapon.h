// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RaycastWeapons.h"
#include "AssaultRifleWeapon.generated.h"

UCLASS()
class SURVIVAL_API AAssaultRifleWeapon : public ARaycastWeapons
{
	GENERATED_BODY()

public:
	AAssaultRifleWeapon();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Fire() override;
	void StopFire();


protected:
	virtual void BeginPlay() override;
	virtual void PlayWeaponEffect() override;

	void StartAutoFire();
	void ExecuteFire();

	virtual void InitializeFireModes() final;
private:
	FTimerHandle AutoFireTimerHandle;
	float FireRate = 0.1f;
};
