// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttackAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"

URangedAttackAbility::URangedAttackAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Ranged"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(AbilityTag);
}

void URangedAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (PlayerCharacterRef && PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		// PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->GetWeaponDataAsset()
	}
	
}

void URangedAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);
}

void URangedAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);
}

void URangedAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
}

void URangedAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
