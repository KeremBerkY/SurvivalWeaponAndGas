// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilitySystemComponent.h"

#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"

#include "Attributes/CharacterAttributeSet.h"


void UCharacterAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& AbilitiesToGrant)
{
	// Eğer Ability listesi boşsa log ekleyelim.
	if (AbilitiesToGrant.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddCharacterAbilities: No abilities to grant."));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("AddCharacterAbilities: Granting %d abilities."), AbilitiesToGrant.Num());
	
	for (const TSubclassOf<UCharacterGameplayAbility>& Ability : AbilitiesToGrant)
	{
		if (!Ability)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddCharacterAbilities: Found a null ability in the list. Skipping..."));
			continue;
		}

		const FString AbilityName = Ability->GetName();

		UE_LOG(LogTemp, Log, TEXT("AddCharacterAbilities: Adding ability %s"), *AbilityName);
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID));
		GiveAbility(AbilitySpec);

		UE_LOG(LogTemp, Log, TEXT("AddCharacterAbilities: Successfully granted ability %s"), *AbilityName);
	}

	UE_LOG(LogTemp, Log, TEXT("AddCharacterAbilities: All abilities have been processed."));
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

// void UCharacterAbilitySystemComponent::AbilityActorInfoSet()
// {
// 	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCharacterAbilitySystemComponent::EffectApplied);
// 	
// }
//
// void UCharacterAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
// {
// 	if (!GEngine)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("GEngine is nullptr, cannot display on-screen messages."));
// 		return;
// 	}
// 	
// 	FGameplayTagContainer TagContainer;
// 	EffectSpec.GetAllAssetTags(TagContainer);
//
// 	for (const FGameplayTag Tag : TagContainer)
// 	{
// 		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
// 		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
// 	}
// }
