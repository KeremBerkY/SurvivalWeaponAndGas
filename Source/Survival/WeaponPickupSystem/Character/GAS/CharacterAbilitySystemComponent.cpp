// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilitySystemComponent.h"

#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"

#include "Attributes/CharacterAttributeSet.h"


void UCharacterAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& AbilitiesToGrant)
{
	if (AbilitiesToGrant.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterAbilities: No abilities to grant."));
		return;
	}
	
	for (const TSubclassOf<UCharacterGameplayAbility>& Ability : AbilitiesToGrant)
	{
		if (!Ability)
		{
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID));
		GiveAbility(AbilitySpec);
	}
}

void UCharacterAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& PassivesToGrant)
{
	for (const TSubclassOf<UCharacterGameplayAbility>& Ability : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		GiveAbility(AbilitySpec);
	}
}

void UCharacterAbilitySystemComponent::InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect)
{
	checkf(AttributeEffect, TEXT("No valid default attributes for this character %s"), *GetAvatarActor()->GetName());
	
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UCharacterAbilitySystemComponent::InitializeSpecialAttributes(const TArray<TSubclassOf<UGameplayEffect>>& SpecialEffects)
{
	for (const TSubclassOf<UGameplayEffect>& Effect : SpecialEffects)
	{
		const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, 1.f, ContextHandle);
		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

