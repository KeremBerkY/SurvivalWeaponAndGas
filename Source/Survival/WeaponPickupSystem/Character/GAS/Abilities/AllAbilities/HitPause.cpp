// Fill out your copyright notice in the Description page of Project Settings.


#include "HitPause.h"

#include "Kismet/GameplayStatics.h"

UHitPause::UHitPause()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.HitPause"));
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
}

void UHitPause::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	
	GetWorld()->GetTimerManager().SetTimer(
		HitPauseTimerHandle,
		this,
		&UHitPause::HitPauseDelay,
		1,
		false
	);
	
}

void UHitPause::HitPauseDelay()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorld()->GetTimerManager().ClearTimer(HitPauseTimerHandle);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UHitPause::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

