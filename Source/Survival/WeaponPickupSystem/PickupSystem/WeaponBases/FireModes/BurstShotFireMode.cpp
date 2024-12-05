// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstShotFireMode.h"

#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"

void UBurstShotFireMode::StartFire(AWeaponBase* Weapon)
{
	SetCurrentWeapon(Weapon);
	
	if (!CurrentWeapon || !CurrentWeapon->CanFire()) return;
	UE_LOG(LogTemp, Warning, TEXT("UBurstShotFireMode::StartFire called!"));
	
	CurrentWeapon->GetFireSettings().BurstShotsRemaining = CurrentWeapon->GetFireSettings().BurstCount;
	CurrentWeapon->GetFireSettings().bBurstFiring = true;
	
	FireShot();
	StartBurstTimer();

}

void UBurstShotFireMode::StopFire(AWeaponBase* Weapon)
{
	if (!Weapon) return;

	CurrentWeapon->GetFireSettings().bBurstFiring = false;

	UWorld* World = GetWorld();
	if (World && CurrentWeapon->GetFireSettings().FireRateTimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(CurrentWeapon->GetFireSettings().FireRateTimerHandle);
	}
	
	CurrentWeapon->StartCooldown();  
}

void UBurstShotFireMode::FireShot()
{
	if (!CurrentWeapon || CurrentWeapon->WeaponAttributes.GetCurrentAmmo() <= 0) return;
	
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
		CurrentWeapon->ExecuteFire();
		UE_LOG(LogTemp, Warning, TEXT("Burst Fire Mode - Shot Fired"));
	}
}

void UBurstShotFireMode::StartBurstTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is null in StartBurstFire."));
		return;
	}
	
	World->GetTimerManager().SetTimer(
	CurrentWeapon->GetFireSettings().FireRateTimerHandle,
	this,
	&UBurstShotFireMode::BurstFireShot,
	CurrentWeapon->GetFireSettings().FireRate,
	true
	);
}

void UBurstShotFireMode::BurstFireShot()
{
	if (CurrentWeapon && CurrentWeapon->GetFireSettings().BurstShotsRemaining > 0)
	{
		FireShot();
		CurrentWeapon->GetFireSettings().BurstShotsRemaining--;
	}
	else // 3 mermi sıkıldı, Burst Tamamlandı
	{
		if (CurrentWeapon->GetFireSettings().bBurstFiring) // Tuş hala basılıysa
		{
			CurrentWeapon->GetFireSettings().BurstShotsRemaining = CurrentWeapon->GetFireSettings().BurstCount;
			FinishedBurstCountTimer();
		}
	}
}

void UBurstShotFireMode::FinishedBurstCountTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is null in StartBurstFire."));
		return;
	}
	
	World->GetTimerManager().SetTimer(
	BurstShotTimerHandle,
	this,
	&UBurstShotFireMode::ResetBurstCount,
	BurstCountCooldown,
	false
	);
}

void UBurstShotFireMode::ResetBurstCount()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() is null in StartBurstFire."));
		return;
	}
	
	if (World && CurrentWeapon->GetFireSettings().FireRateTimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(BurstShotTimerHandle);
	}
}
