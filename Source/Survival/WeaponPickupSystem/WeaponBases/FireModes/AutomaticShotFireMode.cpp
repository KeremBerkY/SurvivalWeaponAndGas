// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticShotFireMode.h"

#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"


void UAutomaticShotFireMode::StartFire(AWeaponBase* Weapon)
{
	SetCurrentWeapon(Weapon);
	
	if (!CurrentWeapon || !CurrentWeapon->CanFire()) return;

	AutomaticFire();
	StartAutomaticTimer();
}

void UAutomaticShotFireMode::StartAutomaticTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is null in StartBurstFire."));
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		CurrentWeapon->GetFireSettings().FireRateTimerHandle,
		this,
		&UAutomaticShotFireMode::AutomaticFire,
		CurrentWeapon->GetFireSettings().FireRate,
		true
	);
}

void UAutomaticShotFireMode::StopFire(AWeaponBase* Weapon)
{
	if (!CurrentWeapon) return;
	
	UWorld* World = GetWorld();
	if (World && CurrentWeapon->GetFireSettings().FireRateTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->GetFireSettings().FireRateTimerHandle);
	}
	
	// CurrentWeapon->GetFireSettings().bCanFire = true;
	// CurrentWeapon->GetFireSettings().bIsFiring = false;
	CurrentWeapon->StartCooldown();
}

void UAutomaticShotFireMode::AutomaticFire()
{
	if (!CurrentWeapon || CurrentWeapon->GetWeaponAttributes().GetCurrentAmmo() <= 0) return;
	
	CurrentWeapon->Fire();
	CurrentWeapon->ExecuteFire();
}


