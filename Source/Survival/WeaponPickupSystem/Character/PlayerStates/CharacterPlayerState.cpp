// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerState.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Attributes/CharacterAttributeSet.h"

ACharacterPlayerState::ACharacterPlayerState()
{
	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;

	CharacterAbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("AbilitySystemComponent");
	CharacterAbilitySystemComponent->SetIsReplicated(true); // not required for single-player gameplay
	CharacterAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // not required for single-player gameplay ! Mixed but Minimal is optimal

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return CharacterAbilitySystemComponent;
}

UCharacterAbilitySystemComponent* ACharacterPlayerState::GetCharacterAbilitySystemComponent() const
{
	return CharacterAbilitySystemComponent;
}

UCharacterAttributeSet* ACharacterPlayerState::GetCharacterAttributes() const
{
	return CharacterAttributes;
}
