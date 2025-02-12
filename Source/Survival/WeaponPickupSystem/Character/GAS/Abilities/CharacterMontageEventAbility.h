// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterGameplayAbility.h"
#include "CharacterMontageEventAbility.generated.h"

class UCharacterAbilityTask_PlayMontageAndWaitForEvent;
class ASurvivalCharacter;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UCharacterMontageEventAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	float MontageRate = 1.0f;

	UPROPERTY()
	UAnimMontage* AbilityMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, Category="Ability")
	TSubclassOf<UGameplayEffect> BlockDamageEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload);

	UFUNCTION()
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload);

	UFUNCTION()
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload);

	UFUNCTION()
	virtual void StartAnimMontage();
	
	UFUNCTION()
	virtual void StopAnimMontage();

	UFUNCTION()
	FGameplayEffectSpecHandle MakeCharacterDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount = 1);

	UFUNCTION()
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UPROPERTY(BlueprintReadOnly)
	ASurvivalCharacter* PlayerCharacterRef;

	UPROPERTY()
	UCharacterAbilityTask_PlayMontageAndWaitForEvent* Task;

	UPROPERTY(EditDefaultsOnly, Category= "Character")
	bool bUseAnimMontageRandom = true;

	UPROPERTY(EditDefaultsOnly, Category= "Character", meta=(EditCondition = "bUseAnimMontageRandom == false", EditConditionHides))
	uint8 UseAnimMontageAtIndex;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer BlockTagList;
	
	UFUNCTION()
	virtual void BlockAbilities(bool IsEnableBlockAbilities);


	
};
