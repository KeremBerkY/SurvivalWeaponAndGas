// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"

#include "GameplayTagContainer.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/MeleeWeapons/MeleeWeapon.h"


UEnemyCombatComponent::UEnemyCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UEnemyCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AMeleeWeapon* InWeaponToRegister, bool bRegisterEquippedWeapon)
{
	Debug::Print(InWeaponTagToRegister.ToString());
	checkf(!EnemyCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString())
	check(InWeaponToRegister);
	
	EnemyCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// InWeaponToRegister->OnWeaponHitTarget.AddUnique(this, &ThisClass::OnHitTargetActor);
	// InWeaponToRegister->OnWeaponPulledFromTarget.AddUnique(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
	if (bRegisterEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AMeleeWeapon* UEnemyCombatComponent::GetEnemyCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (EnemyCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AMeleeWeapon* const* FoundWeapon = EnemyCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

AMeleeWeapon* UEnemyCombatComponent::GetEnemyCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetEnemyCarriedWeaponByTag(CurrentEquippedWeaponTag);
}




