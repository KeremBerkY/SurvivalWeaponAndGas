// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

#include "Survival/SurvivalCharacter.h"


// Sets default values
APistolWeapon::APistolWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();

	// FireSettings.SupportedFireModes = { EFireMode::SingleShot, EFireMode::BurstFire };
}

void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistolWeapon::Equip(ACharacter* Character)
{
	Super::Equip(Character);

	
}

void APistolWeapon::Fire()
{
	Super::Fire();

	float WeaponDamage = WeaponAttributes.GetDamage();
	UE_LOG(LogTemp, Warning, TEXT("Pistol shoot!!: %f"), WeaponDamage);

}

void APistolWeapon::PlayWeaponEffect()
{
	Super::PlayWeaponEffect();
}

