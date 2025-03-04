// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstShotModeComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
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

	FireModeType = EFireMode::BurstShot;

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
			&UBurstShotModeComponent::LoopModeFire,
			RaycastWeaponDataPtr->FireRate,
			true
		);
	}
	
}

void UBurstShotModeComponent::EndFire()
{
	Super::EndFire();

	bool IsCooldownInactive = !OwnerWeaponPtr->GetAttackCooldownActive();
	UE_LOG(LogTemp, Warning, TEXT("IsCooldownInactive: %s"), IsCooldownInactive ? TEXT("true") : TEXT("false"));

	// CanFire() fonksiyonunun tersini logla
	bool IsCannotFire = !OwnerWeaponPtr->CanFire();
	UE_LOG(LogTemp, Warning, TEXT("IsCannotFire: %s"), IsCannotFire ? TEXT("true") : TEXT("false"));
	
	if (GetWorld() && !OwnerWeaponPtr->GetAttackCooldownActive() && !OwnerWeaponPtr->CanFire())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
		
		OwnerWeaponPtr->SetAttackCooldownActive(true);
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&UBurstShotModeComponent::ResetFire,
			RaycastWeaponDataPtr->FireRate,
			false
		);
	}
}

void UBurstShotModeComponent::LoopModeFire()
{
	Super::LoopModeFire();
	
	if (OwnerWeaponPtr.IsValid() && BurstShotRemaining > 0 && !OwnerWeaponPtr->GetHeatComponent()->IsOverHeated())
	{
		BurstShotRemaining--;
		if (!GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag))
		{
			GetCharacterAbilitySystemComponent()->AddLooseGameplayTag(FireTag);
		}
		GetCharacterAbilitySystemComponent()->TryActivateAbilityByClass(OwnerWeaponPtr->GetRaycastWeaponDataAsset()->FireAbility);
		OwnerWeaponPtr->PerformFire();
		Debug::Print(TEXT("BurstFire Loop"));
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
					RaycastWeaponDataPtr->FireRate / 2.f,
					false
				);
			}
		}
		else
		{
			BurstShotRemaining = BurstCount; // Sonradan eklendi!

			GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
			OwnerWeaponPtr->SetCanFire(true);
			OwnerWeaponPtr->SetAttackCooldownActive(false);
		}
	}
}

void UBurstShotModeComponent::ResetFire()
{
	Super::ResetFire();

	if (GetCharacterAbilitySystemComponent())
	{
		GetCharacterAbilitySystemComponent()->RemoveLooseGameplayTag(FireTag);
	}
	
}

void UBurstShotModeComponent::BurstFireCooldown()	
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(
			BurstFireTimerHandle,
			this,
			&UBurstShotModeComponent::LoopModeFire,
			RaycastWeaponDataPtr->FireRate,
			true
		);
	}
}


