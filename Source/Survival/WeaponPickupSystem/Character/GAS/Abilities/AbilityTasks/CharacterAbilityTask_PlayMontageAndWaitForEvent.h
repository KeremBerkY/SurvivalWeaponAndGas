// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CharacterAbilityTask_PlayMontageAndWaitForEvent.generated.h"

/**
 * 
 */

class UCharacterAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class SURVIVAL_API UCharacterAbilityTask_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

	explicit UCharacterAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override; // Runs when the mission is started and plays the animation montage.
	virtual void ExternalCancel() override; // The mission can be cancelled by an external action.
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override; // Runs when the mission is completely finished

public:
	
	/** The montage completely finished playing */
	UPROPERTY(BlueprintAssignable)
	FCharacterPlayMontageAndWaitForEventDelegate OnCompleted;

	/** The montage started blending out */
	UPROPERTY(BlueprintAssignable)
	FCharacterPlayMontageAndWaitForEventDelegate OnBlendOut;

	/** The montage was interrupted */
	UPROPERTY(BlueprintAssignable)
	FCharacterPlayMontageAndWaitForEventDelegate OnInterrupted;

	/** The ability task was explicitly cancelled by another ability */
	UPROPERTY(BlueprintAssignable)
	FCharacterPlayMontageAndWaitForEventDelegate OnCancelled;

	/** One of the triggering gameplay events happened */
	UPROPERTY(BlueprintAssignable)
	FCharacterPlayMontageAndWaitForEventDelegate EventReceived;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCharacterAbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			UAnimMontage* MontageToPlay,
			FGameplayTagContainer EventTags,
			float Rate = 1.f,
			FName StartSection = NAME_None,
			bool bStopWhenAbilityEnds = true,
			float AnimRootMotionTranslationScale = 1.f);

private:

	/** Montage that is playing */
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	/** List of tags to match against gameplay events */
	UPROPERTY()
	FGameplayTagContainer EventTags;

	/** Playback rate */
	UPROPERTY()
	float Rate;

	/** Section to start montage from */
	UPROPERTY()
	FName StartSection;

	/** Modifies how root motion movement to apply */
	UPROPERTY()
	float AnimRootMotionTranslationScale;

	/** Rather montage should be aborted if ability ends */
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();
	
	/** Returns our ability system component */
	UCharacterAbilitySystemComponent* GetTargetASC() const;
	
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;

};
