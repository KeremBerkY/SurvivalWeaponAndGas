// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RangedWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"

UReloadAbility::UReloadAbility()
{
}

void UReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (const UCharacterWeaponComponent* WeaponComponent = PlayerCharacter->GetCharacterWeaponComponent())
		{
			if (ARangedWeapon* CurrentWeapon = Cast<ARangedWeapon>(WeaponComponent->GetCurrentWeapon()))
			{
				if (CurrentWeapon->GetCurrentAmmo() <= CurrentWeapon->GetRangedWeaponDataAsset()->WeaponAmmoAttributes.AmmoInMagazine &&
				CurrentWeapon->GetCurrentAmmo() < CurrentWeapon->GetRangedWeaponDataAsset()->WeaponAmmoAttributes.CurrentAmmo)
				{
					if (CurrentWeapon->GetRangedWeaponDataAsset()->ReloadAnimation)
					{
						AbilityMontage = CurrentWeapon->GetRangedWeaponDataAsset()->ReloadAnimation;
						StartAnimMontage();
					}
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

}

void UReloadAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
}

void UReloadAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UReloadAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
