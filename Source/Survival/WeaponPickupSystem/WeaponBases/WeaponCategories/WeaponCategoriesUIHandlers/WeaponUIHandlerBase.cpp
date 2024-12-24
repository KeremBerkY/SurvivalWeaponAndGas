// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUIHandlerBase.h"


UWeaponUIHandlerBase::UWeaponUIHandlerBase()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponUIHandlerBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UWeaponUIHandlerBase::InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter)
{
}

void UWeaponUIHandlerBase::SetupUI(AWeaponBase* Weapon)
{
}

void UWeaponUIHandlerBase::UpdateUI(AWeaponBase* Weapon)
{
}

void UWeaponUIHandlerBase::RemoveUI(AWeaponBase* Weapon)
{
}

