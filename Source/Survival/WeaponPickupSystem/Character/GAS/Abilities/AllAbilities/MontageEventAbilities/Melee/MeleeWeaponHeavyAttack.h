// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "MeleeWeaponHeavyAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UMeleeWeaponHeavyAttack : public UCharacterMontageEventAbility
{
	GENERATED_BODY()

protected:
	UMeleeWeaponHeavyAttack();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Montages")
	TMap<int32, UAnimMontage*> AttackMontagesMap;

private:
	void HandleComboAndSetMontage();
	void ResetAttackComboCount();
	void SetComboResetTimer();
	void ClearComboResetTimer();
	
	UPROPERTY(EditAnywhere, Category="GameplayCue|Tag", meta=(AllowPrivateAccess="true"))
	FGameplayTag GameplayCueTag;

	UPROPERTY(EditAnywhere, Category="Ability|Effect", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> RageEffect;
	
	UPROPERTY(EditAnywhere, Category="Ability|Effect", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> HealEffect;
	
	FTimerHandle ComboCountResetTimerHandle;

	int32 CurrentHeavyAttackComboCount;
	int32 UsedComboCount;
};
