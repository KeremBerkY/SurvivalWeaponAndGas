// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifleWeapon.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/WeaponBases/FireModes/AutomaticShotFireMode.h"
#include "Survival/WeaponPickupSystem/WeaponBases/FireModes/BurstShotFireMode.h"
#include "Survival/WeaponPickupSystem/WeaponBases/FireModes/FireModeFactory.h"


// Sets default values
AAssaultRifleWeapon::AAssaultRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("AAssaultRifleWeapon constructor called"));

}


void AAssaultRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentFireMode.GetObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Fire Mode initialized: %s"), *CurrentFireMode.GetObject()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Fire Mode is not initialized."));
	}
}


void AAssaultRifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

