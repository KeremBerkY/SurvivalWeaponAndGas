// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilityTask_PlayMontageAndWaitForEvent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UCharacterAbilityTask_PlayMontageAndWaitForEvent::UCharacterAbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::Activate()
{
	Super::Activate();

	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = GetTargetASC())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Bind to event callback
			EventHandle = CharacterAbilitySystemComponent->AddGameplayEventTagContainerDelegate(
				EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnGameplayEvent));

			if (CharacterAbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &ThisClass::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &ThisClass::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetAvatarActor());
				if (PlayerCharacter && (PlayerCharacter->GetLocalRole() == ROLE_Authority ||
					(PlayerCharacter->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() ==
						EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					PlayerCharacter->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UCharacterAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"))
	}

	if (!bPlayedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."),
			   *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::ExternalCancel()
{
	Super::ExternalCancel();

	check(AbilitySystemComponent.IsValid());

	OnAbilityCancelled();

	Super::ExternalCancel();
}

FString UCharacterAbilityTask_PlayMontageAndWaitForEvent::GetDebugString() const
{
	const UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();

		if (const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance(); AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(
		TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay),
		*GetNameSafe(PlayingMontage));
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	if (UCharacterAbilitySystemComponent* CharacterAbilitySystemComponent = GetTargetASC())
	{
		CharacterAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}
	
	Super::OnDestroy(AbilityEnded);
}

UCharacterAbilityTask_PlayMontageAndWaitForEvent* UCharacterAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	FGameplayTagContainer EventTags,
	float Rate,
	FName StartSection,
	bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UCharacterAbilityTask_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UCharacterAbilityTask_PlayMontageAndWaitForEvent>(
		OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags = EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return MyObj;
	
}

bool UCharacterAbilityTask_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	if (AbilitySystemComponent.IsValid() && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

UCharacterAbilitySystemComponent* UCharacterAbilityTask_PlayMontageAndWaitForEvent::GetTargetASC() const
{
	return Cast<UCharacterAbilitySystemComponent>(AbilitySystemComponent);
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			// Reset AnimRootMotionTransitionScale
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
				(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() ==
					EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}
		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage Event Ability->Ability Task sending OnInterrupted"));
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage Event Ability->Ability Task sending OnBlendOut"));
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		else
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

void UCharacterAbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;
		
		EventReceived.Broadcast(EventTag, TempData);
	}
}
