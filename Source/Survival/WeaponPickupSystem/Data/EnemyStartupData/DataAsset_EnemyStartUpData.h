// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UGameplayEffect;
class USurvivalEnemyGameplayAbility;
class UCharacterAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UDataAsset_EnemyStartUpData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf < USurvivalEnemyGameplayAbility > > ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf < USurvivalEnemyGameplayAbility > > ReactiveAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf<UGameplayEffect> > StartUpGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf <USurvivalEnemyGameplayAbility> > EnemyCombatAbilities;
	
	void GrantAbilities(const TArray< TSubclassOf < USurvivalEnemyGameplayAbility > >& InAbilitiesToGive, UCharacterAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
	
};
