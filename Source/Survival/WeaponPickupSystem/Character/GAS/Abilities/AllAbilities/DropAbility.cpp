// Fill out your copyright notice in the Description page of Project Settings.


#include "DropAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"


void UDropAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UCharacterWeaponComponent* CurrentWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent())
		{
			CurrentWeaponComponent->DropWeapon(PlayerCharacter);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UDropAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
