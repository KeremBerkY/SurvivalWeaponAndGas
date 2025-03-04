// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "PistolWeapon.generated.h"

class UAutomaticShotModeComponent;
class UBurstShotModeComponent;

UCLASS()
class SURVIVAL_API APistolWeapon : public ARaycastWeapons
{
	GENERATED_BODY()

public:
	APistolWeapon();
	
	virtual void Tick(float DeltaTime) override;
	void PerformFire();

protected:
	virtual void AddFireModes() override;

private:
	UPROPERTY()
	UBurstShotModeComponent* BurstShotModeComponent;
	UPROPERTY()
	UAutomaticShotModeComponent* AutomaticShotModeComponent;
};
