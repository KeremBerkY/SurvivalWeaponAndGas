// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"
#include "Enemy_MeleeAttack_Base.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UEnemy_MeleeAttack_Base : public USurvivalEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	UEnemy_MeleeAttack_Base();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void HandleApplyDamage(const FGameplayEventData& GameplayEventData);
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere)
	FScalableFloat InDamageScalableFloat;
	
private:
	// UPROPERTY(EditAnywhere, Category="Ability|Effect", meta=(AllowPrivateAccess="true"))
	// TSubclassOf<UGameplayEffect>
	
};
