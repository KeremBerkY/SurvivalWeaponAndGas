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

void UCharacterAbilitySystemComponent::AddCharacterReactiveAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& PassivesToGrant)
{
	for (const TSubclassOf<UCharacterGameplayAbility>& Ability : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		GiveAbility(AbilitySpec);
	}

	// if (PassivesToGrant.IsEmpty())
	// {
	// 	return;
	// }
	//
	// for (const TSubclassOf<UCharacterGameplayAbility>& Ability : PassivesToGrant)
	// {
	// 	if (!Ability) continue;
	//
	// 	FGameplayAbilitySpec AbilitySpec(Ability);
	// 	AbilitySpec.SourceObject = GetAvatarActor();
	// 	AbilitySpec.Level = 1;
	// 	
	// 	GiveAbility(AbilitySpec);
	// }
}

void UCharacterAbilitySystemComponent::InitializeDefaultAttributes(const TArray<TSubclassOf<UGameplayEffect>>& AttributeEffect) // TODO: If you want you can add int32 ApplyLevel for character!
{
	// checkf(AttributeEffect, TEXT("No valid default attributes for this character %s"), *GetAvatarActor()->GetName());
	
	// const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	// const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	// ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	for (const TSubclassOf<UGameplayEffect>& Effect : AttributeEffect)
	{
		const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, 1.f, ContextHandle);
		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
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

bool UCharacterAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitiesSpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitiesSpecs);

	if (!FoundAbilitiesSpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitiesSpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitiesSpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}

