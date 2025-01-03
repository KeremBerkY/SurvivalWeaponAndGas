// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"

void UAimingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PlayerCharacterPtr = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get());
	if (PlayerCharacterPtr.IsValid())
	{
		CameraComponentPtr = PlayerCharacterPtr->GetCharacterCameraComponent();
		LockonComponentPtr = PlayerCharacterPtr->GetLockonComponent();
		if (CameraComponentPtr.IsValid() && LockonComponentPtr.IsValid() && PlayerCharacterPtr->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			CameraComponentPtr->AimingButtonPressed();
			LockonComponentPtr->RemoveFocusCrosshair();
			LockonComponentPtr->EndLockon();
			LockonComponentPtr->EndSelect();
		}
	}
	
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UAimingAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
}

void UAimingAbility::OnInputRelease(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAimingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (PlayerCharacterPtr.IsValid() && CameraComponentPtr.IsValid() && LockonComponentPtr.IsValid())
	{
		if (!LockonComponentPtr->IsLocked())
		{
			LockonComponentPtr->AddFocusCrosshair();
		}
		CameraComponentPtr->AimingButtonReleased();
	}
}
