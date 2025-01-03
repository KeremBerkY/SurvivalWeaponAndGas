// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"

void ULockonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	PlayerCharacterPtr = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get());
	LockonComponentPtr = PlayerCharacterPtr.Get()->GetLockonComponent();


	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ULockonAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
	
}

void ULockonAbility::OnInputRelease(float TimeHeld)
{
	if (!PlayerCharacterPtr.IsValid() || !LockonComponentPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacterPtr veya LockonComponentPtr geçersiz."));
		return;
	}
		
	if (LockonComponentPtr.Get()->IsLocked() && TimeHeld > 1.f)
	{
		LockonComponentPtr.Get()->AddFocusCrosshair();
		LockonComponentPtr.Get()->EndLockon();
		UE_LOG(LogTemp, Warning, TEXT("Lockon Ended!"));
		bLockEnded = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Lockon RELEASED AbilitySystem"));
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void ULockonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (PlayerCharacterPtr.IsValid() && !bLockEnded)
	{
		LockonComponentPtr = PlayerCharacterPtr.Get()->GetLockonComponent();
		if (LockonComponentPtr.IsValid() && !LockonComponentPtr.Get()->IsLocked())
		{
			// LockonComponentPtr.Get()->RemoveFocusCrosshair();
			LockonComponentPtr.Get()->StartLockon();
			UE_LOG(LogTemp, Warning, TEXT("Activateability StartLockon"));
		}
		else
		{
			// TODO: Toggle çağır ve en yakındaki düşmana geçsin.
			UE_LOG(LogTemp, Warning, TEXT("Activateability Toggle"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EndAbility Called LockEnded"));
	}
}
