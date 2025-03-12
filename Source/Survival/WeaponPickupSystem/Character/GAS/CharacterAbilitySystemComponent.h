// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

class UGameplayAbility;
class UCharacterGameplayAbility;
class UCharacterAttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AddCharacterAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterReactiveAbilities(const TArray<TSubclassOf<UCharacterGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TArray<TSubclassOf<UGameplayEffect>>& AttributeEffect);
	void InitializeSpecialAttributes(const TArray<TSubclassOf<UGameplayEffect>>& SpecialEffects);

	UFUNCTION(BlueprintPure, Category = "Survival|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
	
	// void AbilityActorInfoSet();
	// void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
