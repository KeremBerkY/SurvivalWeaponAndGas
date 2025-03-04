// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "RaycastWeaponSwitchAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API URaycastWeaponSwitchAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

protected:
	URaycastWeaponSwitchAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	
#pragma region SwitchTimer
	void TickTimer();
	
	UPROPERTY()
	float RemainingTime;

	UPROPERTY(EditDefaultsOnly, Category = "SwitchTimer")
	float SwitchCoolDownDuration;
	
	FTimerHandle SwitchTimerHandle;
	FTimerHandle ResetTimerHandle;
#pragma endregion

	void ApplyManaCost();
	
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
	TSubclassOf<UGameplayEffect> ManaCostEffect;
};
