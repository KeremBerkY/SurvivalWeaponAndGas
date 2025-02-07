// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_EnemyStartUpData.h"

#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(EnemyCombatAbilities, InASCToGive, ApplyLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf< UGameplayEffect >& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}
	
	// if (!EnemyCombatAbilities.IsEmpty())
	// {
	// 	for (const TSubclassOf< USurvivalEnemyGameplayAbility >& AbilityClass : EnemyCombatAbilities)
	// 	{
	// 		if (!AbilityClass) continue;
	//
	// 		FGameplayAbilitySpec AbilitySpec(AbilityClass);
	// 		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
	// 		AbilitySpec.Level = ApplyLevel;
	// 		
	// 		InASCToGive->GiveAbility(AbilitySpec);
	// 	}
	// }
}

void UDataAsset_EnemyStartUpData::GrantAbilities(const TArray<TSubclassOf<USurvivalEnemyGameplayAbility>>& InAbilitiesToGive, UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf< USurvivalEnemyGameplayAbility >& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
