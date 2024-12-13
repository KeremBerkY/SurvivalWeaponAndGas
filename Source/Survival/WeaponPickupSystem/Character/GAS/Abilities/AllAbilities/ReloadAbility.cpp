// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"

void UReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UCharacterWeaponComponent* WeaponComponent = PlayerCharacter->GetCharacterWeaponComponent())
		{
			if (AWeaponBase* CurrentWeapon = WeaponComponent->GetCurrentWeapon())
			{
				CurrentWeapon->Reload();
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}