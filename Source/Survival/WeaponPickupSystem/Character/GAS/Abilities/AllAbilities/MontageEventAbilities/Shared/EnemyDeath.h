// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"
#include "EnemyDeath.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UEnemyDeath : public USurvivalEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	UEnemyDeath();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Montages")
	TArray<UAnimMontage*> MontagesToPlay;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayCueTag")
	FGameplayTag DeathSoundGameplayCueTag;
	
};
