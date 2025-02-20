// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"
#include "ThrowBack.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UThrowBack : public USurvivalEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
	TWeakObjectPtr<ASurvivalEnemyCharacter> EnemyCharacterPtr;
	

	// void SetEnemyRotationToOriginal();
	void SetEnemyRotationToOriginal(TObjectPtr<const AActor> Instigator, UPrimitiveComponent* EnemyRoot);
	void AdjustCharacterToGround(ASurvivalEnemyCharacter* EnemyCharacter);
};
