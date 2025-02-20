// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticShotModeComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


UAutomaticShotModeComponent::UAutomaticShotModeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	FireTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Weapon.FireMode.Automatic"));
}

void UAutomaticShotModeComponent::Fire()
{
	Super::Fire();

	if (GetCharacterAbilitySystemComponent())
	{
		if (OwnerWeaponPtr->CanFire() && !GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag)) // bIsAutomatic koy ve Automatic de değilse çalışsın. EndFire() da sıfırlarsın.
		{
			OwnerWeaponPtr->SetCanFire(false);

			OwnerWeaponPtr->PerformFire();

			if (!GetWorld()) return;
			
			GetWorld()->GetTimerManager().SetTimer(
				AutomaticFireTimerHandle,
				this,
				&UAutomaticShotModeComponent::AutomaticFire,
				RaycastWeaponDataPtr->FireRate,
				true
			);
		}
	}
	
}

void UAutomaticShotModeComponent::EndFire()
{
	Super::EndFire();

	if (GetWorld() && !OwnerWeaponPtr->CanFire() && !OwnerWeaponPtr->GetAttackCooldownActive())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
		
		OwnerWeaponPtr->SetAttackCooldownActive(true);
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&UAutomaticShotModeComponent::ResetFire,
			RaycastWeaponDataPtr->FireRate,
			false
		);
	}

}

void UAutomaticShotModeComponent::AutomaticFire()
{
	if (OwnerWeaponPtr)
	{
		if (GetCharacterAbilitySystemComponent())
		{
			if (!GetCharacterAbilitySystemComponent()->HasMatchingGameplayTag(FireTag))
			{
				GetCharacterAbilitySystemComponent()->AddLooseGameplayTag(FireTag);
			}
		}
		if (OwnerWeaponPtr->GetRaycastWeaponDataAsset())
		{
			GetCharacterAbilitySystemComponent()->TryActivateAbilityByClass(OwnerWeaponPtr->GetRaycastWeaponDataAsset()->FireAbility);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot Reach RaycastData!"));
		}

		OwnerWeaponPtr->SetCanFire(false);
		OwnerWeaponPtr->PerformFire();
	}
}

void UAutomaticShotModeComponent::ResetFire() const
{
	if (GetCharacterAbilitySystemComponent())
	{
		GetCharacterAbilitySystemComponent()->RemoveLooseGameplayTag(FireTag);
	}

	OwnerWeaponPtr->SetCanFire(true);
	OwnerWeaponPtr->SetAttackCooldownActive(false);
	UE_LOG(LogTemp, Log, TEXT("Ready to fire again!"));
}

