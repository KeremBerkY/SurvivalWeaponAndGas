// Fill out your copyright notice in the Description page of Project Settings.


#include "FireModeBaseComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
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
			RaycastWeaponDataPtr = MakeWeakObjectPtr(WeaponData);
			
		}
		OwnerWeaponPtr = MakeWeakObjectPtr(Weapon);
	}
	
	
}

UCharacterAbilitySystemComponent* UFireModeBaseComponent::GetCharacterAbilitySystemComponent() const
{
	return OwnerWeaponPtr->GetOwningCharacter()->GetCharacterAbilitySystemComponent();
}

void UFireModeBaseComponent::ResetFire()
{
	OwnerWeaponPtr->SetCanFire(true);
	OwnerWeaponPtr->SetAttackCooldownActive(false);
}

void UFireModeBaseComponent::LoopModeFire()
{
	if (!OwnerWeaponPtr->GetOwningCharacter()->GetLockonComponent()->IsLocked())
	{
		if (!OwnerWeaponPtr->GetOwningCharacter()->GetCharacterCameraComponent()->IsAiming())
		{
			ResetFire();
		}
	}

}

void UFireModeBaseComponent::Fire()
{
	IFire::Fire();
}

void UFireModeBaseComponent::EndFire()
{
	IFire::EndFire();
}

