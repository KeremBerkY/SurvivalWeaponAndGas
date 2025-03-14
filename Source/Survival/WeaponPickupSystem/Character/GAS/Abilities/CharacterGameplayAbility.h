// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityInputs/GASAbilityInputID.h"

#include "CharacterGameplayAbility.generated.h"

/**
 * 
 */

class UCharacterAbilitySystemComponent;
class ASurvivalCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityEndedDelegate, UCharacterGameplayAbility*, InAbility, bool,
                                             bIsCancelled);

UCLASS()
class SURVIVAL_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCharacterGameplayAbility();

	FORCEINLINE bool HasCooldown() const { return bHasCooldown; }
	FORCEINLINE float GetCooldownDuration() const { return FinalCooldownDuration; };
	FORCEINLINE FGameplayTagContainer GetCooldownTagContainer() const { return CooldownTags; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	bool bAutoActivateWhenGranted;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FAbilityEndedDelegate OnAbilityEnded;
	
	/** Use this override if Auto Activate is set */
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	ASurvivalCharacter* GetPlayerCharacterFromCharacterGameplayAbility();

protected:
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	bool bHasCooldown;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

	TWeakObjectPtr<ASurvivalCharacter> CachedSurvivalCharacter;
	TWeakObjectPtr<APlayerController> CachedSurvivalCharacterController;

	UFUNCTION(BlueprintPure, Category = "Survival|Ability")
	UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponentFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "Survival|Ability")
	APlayerController* GetCharacterControllerFromActorInfo();

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	void CancelCooldown() const;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
private:
	FGameplayTag CooldownTag;
	mutable FActiveGameplayEffectHandle ActiveCooldownEffectHandle;
	
	mutable float FinalCooldownDuration;
	
};
