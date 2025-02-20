// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RangedWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"

void UReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (const UCharacterWeaponComponent* WeaponComponent = PlayerCharacter->GetCharacterWeaponComponent())
		{
			if (ARangedWeapon* CurrentWeapon = Cast<ARangedWeapon>(WeaponComponent->GetCurrentWeapon()))
			{
				if (CurrentWeapon->GetCurrentAmmo() <= CurrentWeapon->GetRangedWeaponDataAsset()->WeaponAmmoAttributes.AmmoInMagazine)
				{
					CurrentWeapon->Reload();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo >= AmmoInMAgazıne Promblem! CurrentAmmo: %d AmmoInMagazıne: %d"),
						CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetRangedWeaponDataAsset()->WeaponAmmoAttributes.AmmoInMagazine)
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
					return;
				}
				
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
