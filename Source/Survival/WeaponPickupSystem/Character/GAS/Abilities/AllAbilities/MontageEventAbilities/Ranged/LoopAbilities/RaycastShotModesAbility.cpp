// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastShotModesAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/SharedComponents/Combat/SurvivalCharacterCombatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

URaycastShotModesAbility::URaycastShotModesAbility()
{}

void URaycastShotModesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// const auto WeaponRef = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	// if (PlayerCharacterRef && WeaponRef)
	// {
	// 	RaycastWeaponPtr = MakeWeakObjectPtr(Cast<ARaycastWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()));
	// 	const auto RaycastWeaponData = RaycastWeaponPtr->GetRaycastWeaponDataAsset();
	// 	
	// 	if (!RaycastWeaponPtr.IsValid() || RaycastWeaponPtr->GetCurrentAmmo() <= 0 ||
	// 		RaycastWeaponPtr->GetHeatComponent()->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity ||
	// 		RaycastWeaponPtr->GetHeatComponent()->IsOverHeated())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("EndAbility Called Before Montage!!!"));
	// 		WeaponRef->EndAttack();
	// 		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	// 		return;
	// 	};
	// 	
	// 	AbilityMontage = RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAnimMontages.FireMontage;
	// 	MontageRate = 1 + (RaycastWeaponPtr->GetRaycastWeaponDataAsset()->FireRate);
	//
	// 	StartAnimMontage();
	// 	PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->Attack();
	// }
}

void URaycastShotModesAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
	
	// if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	// {
	// 	Super::HandleApplyDamage(Payload);
	// }
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}

void URaycastShotModesAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void URaycastShotModesAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastShotModesAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
