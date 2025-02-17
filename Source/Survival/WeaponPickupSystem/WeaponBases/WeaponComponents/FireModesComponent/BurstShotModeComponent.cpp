// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstShotModeComponent.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"


UBurstShotModeComponent::UBurstShotModeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BurstShotRemaining = 3;
	BurstCount = 3;

	FireTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Weapon.FireMode.Burst"));

}

void UBurstShotModeComponent::Fire()
{
	Super::Fire();
	
	if (OwnerWeaponPtr->CanFire() && !GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag))
	{
		OwnerWeaponPtr->SetCanFire(false);

		OwnerWeaponPtr->PerformFire();
		BurstShotRemaining--;

		if (!GetWorld()) return;

		GetWorld()->GetTimerManager().SetTimer(
			BurstFireTimerHandle,
			this,
			&UBurstShotModeComponent::BurstFire,
			RaycastWeaponDataPtr.Get()->FireRate,
			true
		);
	}
	
}

void UBurstShotModeComponent::EndFire()
{
	Super::EndFire();

	if (GetWorld() && !OwnerWeaponPtr->GetAttackCooldownActive() && !OwnerWeaponPtr->CanFire())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
		
		OwnerWeaponPtr->SetAttackCooldownActive(true);
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&UBurstShotModeComponent::ResetFire,
			RaycastWeaponDataPtr.Get()->FireRate,
			false
		);
	}
}

void UBurstShotModeComponent::BurstFire()
{
	if (OwnerWeaponPtr.IsValid() && BurstShotRemaining > 0 && !OwnerWeaponPtr->GetHeatComponent()->IsOverHeated())
	{
		BurstShotRemaining--;
		OwnerWeaponPtr->PerformFire();
		if (!GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag))
		{
			GetCharacterAbilitySystemComponent()->AddLooseGameplayTag(FireTag);
		}
		GetCharacterAbilitySystemComponent()->TryActivateAbilityByClass(OwnerWeaponPtr.Get()->GetRaycastWeaponDataAsset()->FireAbility);
	}
	else
	{
		if (!OwnerWeaponPtr->GetHeatComponent()->IsOverHeated())
		{
			BurstShotRemaining = BurstCount;
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					BurstCoolDownTimerHandle,
					this,
					&UBurstShotModeComponent::BurstFireCooldown,
					RaycastWeaponDataPtr.Get()->FireRate / 2.f,
					false
				);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
			OwnerWeaponPtr->SetCanFire(true);
			OwnerWeaponPtr->SetAttackCooldownActive(false);
		}
	}
}

void UBurstShotModeComponent::ResetFire() const
{
	if (GetCharacterAbilitySystemComponent())
	{
		GetCharacterAbilitySystemComponent()->RemoveLooseGameplayTag(FireTag);
	}
	
	OwnerWeaponPtr->SetCanFire(true);
	OwnerWeaponPtr->SetAttackCooldownActive(false);
	UE_LOG(LogTemp, Log, TEXT("Ready to fire again!"));
}

void UBurstShotModeComponent::BurstFireCooldown()	
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(
			BurstFireTimerHandle,
			this,
			&UBurstShotModeComponent::BurstFire,
			RaycastWeaponDataPtr.Get()->FireRate,
			true
		);
	}
}


