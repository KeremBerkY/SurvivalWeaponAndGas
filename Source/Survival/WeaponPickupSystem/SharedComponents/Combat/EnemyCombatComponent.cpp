// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
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

	InWeaponToRegister->OnWeaponHitTarget.AddUniqueDynamic(this, &ThisClass::EnemyOnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.AddUniqueDynamic(this, &ThisClass::EnemyOnWeaponPulledFromTargetActor);
	
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

void UEnemyCombatComponent::ToggleEnemyWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		const AWeaponBase* WeaponToToggle = GetEnemyCurrentEquippedWeapon();

		check(WeaponToToggle);
		
		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			if (WeaponToToggle->GetWeaponHandleCollisionBox())
			{
				WeaponToToggle->GetWeaponHandleCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (WeaponToToggle->GetWeaponHandleCollisionBox())
			{
				WeaponToToggle->GetWeaponHandleCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				
			}
			
			OverlappedActors.Empty(); 
		}
	}
}

void UEnemyCombatComponent::EnemyOnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	// TODO: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && bIsMyAttackUnblockable)
	{
		// TODO: Check if the block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	if (bIsValidBlock)
	{
		// TODO: Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")),
			EventData
		);
	}
}

void UEnemyCombatComponent::EnemyOnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}




