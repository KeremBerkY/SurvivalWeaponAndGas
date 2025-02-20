// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"

#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RangedWeaponData.h"


ARangedWeapon::ARangedWeapon()
{

	PrimaryActorTick.bCanEverTick = true;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	bCanFire = true;
	bIsInitialized = false;

}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsInitialized) // Deneme
	{
		if (URangedWeaponData* WeaponData = Cast<URangedWeaponData>(GetWeaponDataAsset()))
		{
			RangedWeaponData = WeaponData;
			InitializeAmmo();
			bIsInitialized = true;
		}
	}
}


void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangedWeapon::PerformFire()
{
	// if (!bIsInitialized) // Deneme
	// {
	// 	if (URangedWeaponData* WeaponData = Cast<URangedWeaponData>(GetWeaponDataAsset()))
	// 	{
	// 		RangedWeaponData = WeaponData;
	// 		InitializeAmmo();
	// 		bIsInitialized = true;
	// 	}
	// }
}

void ARangedWeapon::InitializeAmmo() // Deneme
{
	SetTotalAmmo(RangedWeaponData->WeaponAmmoAttributes.AmmoCapacity);
	SetCurrentAmmo(RangedWeaponData->WeaponAmmoAttributes.CurrentAmmo);
}


void ARangedWeapon::Attack()
{
	Super::Attack();


}

void ARangedWeapon::EndAttack()
{
	Super::EndAttack();
}


void ARangedWeapon::Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading is already in progress."));
		return;
	}
	
	if (CurrentAmmo >= RangedWeaponData->WeaponAmmoAttributes.AmmoInMagazine)
	{
		UE_LOG(LogTemp, Warning, TEXT("The magazine is already loaded!"));
		return;
	}
	
	if (RangedWeaponData->WeaponAmmoAttributes.AmmoCapacity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough ammo!"));
		return;
	}
	
	bIsReloading = true;
	
	int32 AmmoNeeded = RangedWeaponData->WeaponAmmoAttributes.AmmoInMagazine - CurrentAmmo;
	int32 AmmoToReload = FMath::Min(AmmoNeeded, RangedWeaponData->WeaponAmmoAttributes.AmmoCapacity);
	
	CurrentAmmo += AmmoToReload;
	SetCurrentAmmo(CurrentAmmo);
	
	// RangedWeaponData->WeaponAmmoAttributes.AmmoCapacity -= AmmoToReload;
	TotalAmmo -= AmmoToReload; // DENEME
	SetTotalAmmo(TotalAmmo);

	OnAmmoChange.Broadcast(CurrentAmmo, TotalAmmo	);
	
	// UE_LOG(LogTemp, Log, TEXT("Reloaded: Magazine: %d, Total Ammo: %d"), CurrentAmmo, RangedWeaponData->WeaponAmmoAttributes.AmmoCapacity);
	UE_LOG(LogTemp, Log, TEXT("Reloaded: Magazine: %d, Total Ammo: %d"), CurrentAmmo, TotalAmmo); // DENEME
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ARangedWeapon::FinishReload, ReloadDuration, false);
}

void ARangedWeapon::FinishReload()
{
	bIsReloading = false;
	UE_LOG(LogTemp, Log, TEXT("Reload complete."));
}

void ARangedWeapon::ToggleFireMode()
{
	
}