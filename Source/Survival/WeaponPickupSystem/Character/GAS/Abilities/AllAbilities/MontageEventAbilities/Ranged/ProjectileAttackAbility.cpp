// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttackAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


UProjectileAttackAbility::UProjectileAttackAbility()
{
}

void UProjectileAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const auto WeaponRef = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	if (PlayerCharacterRef && WeaponRef)
	{
		ProjectileWeaponPtr = MakeWeakObjectPtr(Cast<AProjectileWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()));
		const auto ProjectileWeaponDataAsset = ProjectileWeaponPtr->GetProjectileWeaponDataAsset();
		
		check(ProjectileWeaponDataAsset);
	
		AbilityMontage = ProjectileWeaponPtr->GetProjectileWeaponDataAsset()->WeaponAnimMontages;
		MontageRate = 1 + (ProjectileWeaponPtr->GetProjectileWeaponDataAsset()->FireRate);
		// bIsStopFire = false;
		StartAnimMontage();
		ProjectileWeaponPtr->Attack();
		
		// UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		// 	this, FGameplayTag::RequestGameplayTag(FName("Weapon.State.StopFire")));
		// WaitTask->EventReceived.AddDynamic(this, &UProjectileAttackAbility::StopFire);
		// WaitTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
}

void UProjectileAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
}

void UProjectileAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UProjectileAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UProjectileAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// void UProjectileAttackAbility::StopFire(FGameplayEventData GameplayEventData)
// {
// }

