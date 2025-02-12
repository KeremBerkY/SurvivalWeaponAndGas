// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReact.h"

#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"

UHitReact::UHitReact()
{
	bHasHitReactMontagesToPlay = true;
	FaceAttacker = true;

	// FAbilityTriggerData TriggerData;
	// TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.HitReact"));
	// TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	//
	// // Yeteneğe trigger'ı ekle
	// Triggers.Add(TriggerData);
}

void UHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FaceAttacker)
	{
		// GetEnemyCharacterFromActorInfo()->SetActorRotation();
		// GetEnemyCharacterFromActorInfo()->GetActorLocation();
	}
	
	if (bHasHitReactMontagesToPlay)
	{
		int32 RandomIndex = FMath::RandRange(0, MontagesToPlay.Num() - 1);
		AbilityMontage = MontagesToPlay[RandomIndex];
		
		StartAnimMontage();
	}
	else
	{	
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UHitReact::EndAbility, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		
		GetWorld()->GetTimerManager().SetTimer(HitReactTimerHandle, TimerDel, 0.2f, true);
	}
}

void UHitReact::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.HitReact")))
	{
		
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHitReact::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHitReact::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UHitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
