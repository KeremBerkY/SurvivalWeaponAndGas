// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "SurvivalEnemyGameplayAbility.generated.h"

class ASurvivalEnemyCharacter;
/**
 * 
 */
UCLASS()
class SURVIVAL_API USurvivalEnemyGameplayAbility : public UCharacterMontageEventAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Survival|Ability")
	ASurvivalEnemyCharacter* GetEnemyCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Survival|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<ASurvivalEnemyCharacter> CachedEnemyCharacter;
};
