// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "ShotgunWeapon.generated.h"

class UBurstShotModeComponent;

UCLASS()
class SURVIVAL_API AShotgunWeapon : public ARaycastWeapons
{
	GENERATED_BODY()

public:
	AShotgunWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PerformFire() override;
	virtual void AddFireModes() override;

private:
	UPROPERTY()
	UBurstShotModeComponent* BurstShotModeComponent;

};
