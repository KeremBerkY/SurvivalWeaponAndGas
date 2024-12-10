// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleShotFireMode.h"

#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"

void USingleShotFireMode::StartFire(AWeaponBase* Weapon)
{
	SetCurrentWeapon(Weapon);
	
	if (CurrentWeapon->GetFireSettings().bIsFiring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already firing, cannot fire again until released."));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Single Shot Fire Mode - Fire once"));
	
	CurrentWeapon->Fire();
	SingleShotFire();
	CurrentWeapon->GetFireSettings().bIsFiring = true;
}

void USingleShotFireMode::StopFire(AWeaponBase* Weapon)
{
	if (!Weapon) return;
	
	CurrentWeapon->GetFireSettings().bIsFiring = false;
	UE_LOG(LogTemp, Warning, TEXT("Single Shot Fire Mode - Stopped"));;
}

void USingleShotFireMode::SingleShotFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ExecuteFire();  // Silah efektleri ve geri tepme
		CurrentWeapon->StartCooldown();
	}
}
