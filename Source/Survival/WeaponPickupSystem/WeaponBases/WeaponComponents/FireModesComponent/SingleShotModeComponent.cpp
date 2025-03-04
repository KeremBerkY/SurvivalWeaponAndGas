// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleShotModeComponent.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


USingleShotModeComponent::USingleShotModeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FireTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Weapon.FireMode.SingleShot"));

	FireModeType = EFireMode::SingleShot;

}

void USingleShotModeComponent::Fire()
{
	Super::Fire();

	if (OwnerWeaponPtr->CanFire())
	{
		OwnerWeaponPtr->SetCanFire(false);
		
		OwnerWeaponPtr->PerformFire();

		if (!GetWorld()) return;

		if (GetCharacterAbilitySystemComponent())
		{
			if (!GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag))
			{
				GetCharacterAbilitySystemComponent()->AddLooseGameplayTag(FireTag);
			}
		}
		
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&USingleShotModeComponent::ResetFire,
			RaycastWeaponDataPtr->FireRate,
			false
		);
	}
	
}

void USingleShotModeComponent::ResetFire()
{
	Super::ResetFire();

	if (GetCharacterAbilitySystemComponent())
	{
		GetCharacterAbilitySystemComponent()->RemoveLooseGameplayTag(FireTag);
	}
	
}


void USingleShotModeComponent::EndFire()
{
	Super::EndFire();
	
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle))
	{
		OwnerWeaponPtr->SetAttackCooldownActive(true);
	}
	else
	{
		OwnerWeaponPtr->SetAttackCooldownActive(false);
	}

}


