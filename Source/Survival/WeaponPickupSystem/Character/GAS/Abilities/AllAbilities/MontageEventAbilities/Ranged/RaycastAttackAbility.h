// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "RaycastAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API URaycastAttackAbility : public UCharacterMontageEventAbility
{
	GENERATED_BODY()

protected:
	URaycastAttackAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void HandleApplyDamage(const FGameplayEventData& Payload);
private:
	
	
};
