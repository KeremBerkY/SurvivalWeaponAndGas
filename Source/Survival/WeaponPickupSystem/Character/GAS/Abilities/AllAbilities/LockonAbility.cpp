// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastFocusWidget/RaycastFocusStages/GetOutFromTargetWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"

ULockonAbility::ULockonAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Lockon"));
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
}

void ULockonAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	PlayerCharacterPtr = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get());
	LockonComponentPtr = PlayerCharacterPtr.Get()->GetLockonComponent();

	const auto CurrentWeapon = PlayerCharacterPtr.Get()->GetCharacterWeaponComponent()->GetCurrentWeapon();
	if (!CurrentWeapon)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	WeaponCategory = CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory;
	
	if (!PlayerCharacterPtr.IsValid() || !LockonComponentPtr.IsValid() || WeaponCategory != EWeaponCategory::Ewc_RaycastWeapons)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	PlayerCharacterPtr->GetCharacterCameraComponent()->AimingButtonReleased();	

	bIsHolding = true;
    bLockEnded = false;
    StartTimer();
	
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ULockonAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
}

void ULockonAbility::StartTimer()
{
	if (bIsHolding)
	{
		StartTime = GetWorld()->TimeSeconds;
		GetWorld()->GetTimerManager().SetTimer(LockonTimerHandle, this, &ULockonAbility::CheckTimeHeld, 0.1f, true);
	}
}

void ULockonAbility::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(LockonTimerHandle);
	bIsHolding = false;
}

void ULockonAbility::CheckTimeHeld()
{
	if (!bIsHolding || bLockEnded)
	{
		StopTimer();
		return;
	}

	const float ElapsedTime = GetWorld()->TimeSeconds - StartTime;
	UE_LOG(LogTemp, Warning, TEXT("Geçen Süre: %.2f saniye"), ElapsedTime);

	SetRadialSliderWidgetValue(ElapsedTime);
	
	if (ElapsedTime >= 1.0f)
	{
		if (LockonComponentPtr.IsValid() && LockonComponentPtr->IsLocked())
		{
			// LockonComponentPtr->AddFocusCrosshair();
			LockonComponentPtr->EndLockon();
			UE_LOG(LogTemp, Warning, TEXT("Lockon Ended Automatically due to TimeHeld >= 1.0f!"));
			bLockEnded = true;
			StopTimer();
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}

void ULockonAbility::SetRadialSliderWidgetValue(const float ElapsedTime) const
{
	PlayerCharacterPtr.Get()->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->GetGetOutFromTargetWidget()->SetRadialSliderValue(ElapsedTime);
}

void ULockonAbility::OnInputRelease(float TimeHeld)
{
	if (!PlayerCharacterPtr.IsValid() || !LockonComponentPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacterPtr veya LockonComponentPtr geçersiz."));
		return;
	}

	if (bLockEnded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability zaten tamamlandı, OnInputRelease tetiklenmeyecek."));
		return;
	}

	ResetRadialSliderWidgetValue();
	StopTimer(); 
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void ULockonAbility::ResetRadialSliderWidgetValue() const
{
	PlayerCharacterPtr.Get()->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->GetGetOutFromTargetWidget()->ResetRadialSliderValue();
}

void ULockonAbility::CheckCurrentWeaponAndCategory()
{
	
	
}

void ULockonAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!PlayerCharacterPtr->GetCharacterWeaponComponent()->GetCurrentWeapon()) { return; }
	
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
			LockonComponentPtr->StartToLookNearestEnemy();
			UE_LOG(LogTemp, Warning, TEXT("Activateability Toggle"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EndAbility Called LockEnded"));
	}
}

