// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"


void USprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCharacterAbilitySystemComponent* CharacterASC = Cast<UCharacterAbilitySystemComponent>(ActorInfo->AbilitySystemComponent);
	if (!CharacterASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		DefaultMovementSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("DefaultMovementSpeed saved: %f"), DefaultMovementSpeed);
	}
	

	ActiveSprintEffectHandle = ApplyGameplayEffect(CharacterASC, SprintEffect, 1);

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &USprintAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
}

FActiveGameplayEffectHandle USprintAbility::ApplyGameplayEffect(UCharacterAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 Level)
{
	if (!AbilitySystem || !GameplayEffectClass)
	{
		return FActiveGameplayEffectHandle();
	}
	
	FGameplayEffectSpecHandle  EffectSpecHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffectClass, 1, AbilitySystem->MakeEffectContext());
	if (EffectSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint Effect Applied"));

		return AbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

	
	return FActiveGameplayEffectHandle();
	
}

void USprintAbility::OnInputRelease(float TimeHeld)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Released after %f seconds"), TimeHeld);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void USprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		
		Character->GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
		UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed reverted to: %f"), DefaultMovementSpeed);
		
	}

	if (UCharacterAbilitySystemComponent* CharacterASC = Cast<UCharacterAbilitySystemComponent>(ActorInfo->AbilitySystemComponent))
	{
		if (ActiveSprintEffectHandle.IsValid())
		{
			CharacterASC->RemoveActiveGameplayEffect(ActiveSprintEffectHandle);
		}

		UE_LOG(LogTemp, Warning, TEXT("Sprint END"));
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
