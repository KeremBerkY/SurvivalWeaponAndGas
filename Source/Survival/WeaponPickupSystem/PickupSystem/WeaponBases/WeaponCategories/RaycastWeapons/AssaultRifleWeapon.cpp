// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifleWeapon.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/FireModes/AutomaticShotFireMode.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/FireModes/BurstShotFireMode.h"


// Sets default values
AAssaultRifleWeapon::AAssaultRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("AAssaultRifleWeapon constructor called"));

}


void AAssaultRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	InitializeFireModes();

	if (CurrentFireMode.GetObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Fire Mode initialized: %s"), *CurrentFireMode.GetObject()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Fire Mode is not initialized."));
	}
	
	// FireSettings.SupportedFireModes = { EFireMode::SingleShot, EFireMode::BurstFire, EFireMode::Automatic };
	// FireSettings.CurrentFireModeIndex = 0;
}

void AAssaultRifleWeapon::InitializeFireModes()
{
	Super::InitializeFireModes();
	
	
	TScriptInterface<IFireMode> BurstFireMode;
	BurstFireMode.SetObject(NewObject<UBurstShotFireMode>(this));
	BurstFireMode.SetInterface(Cast<IFireMode>(BurstFireMode.GetObject()));
	FireModeMap.Add(EFireMode::BurstFire, BurstFireMode);
	
	TScriptInterface<IFireMode> AutomaticFireMode;
	AutomaticFireMode.SetObject(NewObject<UAutomaticShotFireMode>(this));
	AutomaticFireMode.SetInterface(Cast<IFireMode>(AutomaticFireMode.GetObject()));
	FireModeMap.Add(EFireMode::Automatic, AutomaticFireMode);
}


void AAssaultRifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssaultRifleWeapon::Equip(ACharacter* Character)
{
	Super::Equip(Character);
	

}

void AAssaultRifleWeapon::Fire()
{
	Super::Fire();

	// float WeaponDamage = WeaponAttributes.GetDamage();
	// UE_LOG(LogTemp, Warning, TEXT("AssaultRifle shoot!!: %f"), WeaponDamage);
	
}

void AAssaultRifleWeapon::StopFire()
{
}


void AAssaultRifleWeapon::StartAutoFire()
{
}

void AAssaultRifleWeapon::ExecuteFire()
{
}

void AAssaultRifleWeapon::PlayWeaponEffect()
{
	Super::PlayWeaponEffect();
}

