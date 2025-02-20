// Fill out your copyright notice in the Description page of Project Settings.


#include "FireModeBaseComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


UFireModeBaseComponent::UFireModeBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UFireModeBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ARaycastWeapons* Weapon = Cast<ARaycastWeapons>(GetOwner()))
	{
		if (URaycastWeaponData* WeaponData = Cast<URaycastWeaponData>(Weapon->GetWeaponDataAsset()))
		{
			RaycastWeaponDataPtr = WeaponData;
			
		}
		OwnerWeaponPtr = Weapon;
	}
	
	
}

UCharacterAbilitySystemComponent* UFireModeBaseComponent::GetCharacterAbilitySystemComponent() const
{
	return OwnerWeaponPtr->GetOwningCharacter()->GetCharacterAbilitySystemComponent();
}

void UFireModeBaseComponent::Fire()
{
	IFire::Fire();
}

void UFireModeBaseComponent::EndFire()
{
	IFire::EndFire();
}

