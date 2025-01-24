// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "MeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UMeleeAttackAbility : public UCharacterMontageEventAbility
{
	GENERATED_BODY()

protected:
	UMeleeAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	virtual void StopAttack(FGameplayEventData GameplayEventData);

	UFUNCTION()
	virtual void AttackSeq();
	
	UFUNCTION()
	virtual void Attack();

};
