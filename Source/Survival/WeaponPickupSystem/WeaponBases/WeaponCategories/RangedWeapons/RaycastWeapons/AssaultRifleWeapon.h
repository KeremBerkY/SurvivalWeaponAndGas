// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "AssaultRifleWeapon.generated.h"

class UBurstShotModeComponent;
class UAutomaticShotModeComponent;
class URaycastWeaponData;


UCLASS()
class SURVIVAL_API AAssaultRifleWeapon : public ARaycastWeapons
{
	GENERATED_BODY()

public:
	AAssaultRifleWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void PerformFire() override;
	virtual void AddFireModes() override;

private:
	UPROPERTY()
	UAutomaticShotModeComponent* AutomaticShotModeComponent;
	UPROPERTY()
	UBurstShotModeComponent* BurstShotModeComponent;
	
};
