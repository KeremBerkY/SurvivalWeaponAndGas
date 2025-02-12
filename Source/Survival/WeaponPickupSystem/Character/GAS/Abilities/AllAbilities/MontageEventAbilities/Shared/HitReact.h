// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"
#include "HitReact.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UHitReact : public USurvivalEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	UHitReact();
	
	void K2_ActivateAbilityFromEvent(const FGameplayEventData& EventData);

	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasHitReactMontagesToPlay;

	bool FaceAttacker;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> MontagesToPlay;

	FTimerHandle HitReactTimerHandle;
};
