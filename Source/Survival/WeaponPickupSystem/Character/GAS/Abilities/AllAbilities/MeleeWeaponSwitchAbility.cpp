// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponSwitchAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"

void UMeleeWeaponSwitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		const UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();
		if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon())
		{
			PlayerCharacter->GetWeaponInventory()->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), PlayerCharacter, EWeaponCategory::Ewc_MeleeWeapons);
			PlayerCharacter->GetCharacterWeaponComponent()->UpdateLastSwitchTime();
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMeleeWeaponSwitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
