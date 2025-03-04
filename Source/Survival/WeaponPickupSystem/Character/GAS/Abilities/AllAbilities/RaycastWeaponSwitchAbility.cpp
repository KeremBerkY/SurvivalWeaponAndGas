// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeaponSwitchAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponInventory/WeaponSwitchWidget/WeaponSwitchCooldownWidget.h"

URaycastWeaponSwitchAbility::URaycastWeaponSwitchAbility()
{
	SwitchCoolDownDuration = 2.0f; // it should be same as WeaponSwitchCooldown from CharacterWeaponComponent
	RemainingTime = SwitchCoolDownDuration;
}

void URaycastWeaponSwitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();

		if (!CharacterWeaponComponent->CanSwitchWeapon())
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
		
		if (const auto WeaponInventory = PlayerCharacter->GetWeaponInventory())
		{
			
			if (WeaponInventory->HasWeaponInCategory(EWeaponCategory::Ewc_RaycastWeapons))
			{
				// && CharacterWeaponComponent->CanSwitchWeapon()
				if (CharacterWeaponComponent  && !PlayerCharacter->GetLockonComponent()->IsLocked())
				{
					WeaponInventory->SwapToBackWeapon(
						CharacterWeaponComponent->GetCurrentWeapon(),
						PlayerCharacter,
						EWeaponCategory::Ewc_RaycastWeapons
					);
					CharacterWeaponComponent->UpdateLastSwitchTime();
				}
			}
			else
			{
				if (!CharacterWeaponComponent->GetCurrentWeapon() || !CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset())
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
					return;
				}
				
				if (!CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()) // Burada Error alıyosun WeaponDataAssetten dolayı olsa gerek.
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
					return;
				}
				if (CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_RaycastWeapons)
				{
					if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
					{
						WeaponInventory->SwapToBackWeapon(
							CharacterWeaponComponent->GetCurrentWeapon(),
							PlayerCharacter,
							EWeaponCategory::Ewc_RaycastWeapons
						);
						CharacterWeaponComponent->UpdateLastSwitchTime();
					}
				}
				else
				{
					WeaponInventory->SwapToBackWeapon(
						CharacterWeaponComponent->GetCurrentWeapon(),
						PlayerCharacter,
						CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory
					);
					CharacterWeaponComponent->UpdateLastSwitchTime();
				}
			}
		}
	}

	if (ManaCostEffect)
	{
		ApplyManaCost();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ManaCostEffect bulunamadı!"));
	}
	
	GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalHUD()->GetMainHUDWidget()
			->GetGameHUDWidget()->GetWeaponSwitchCooldownWidget()->StartCooldown();
	
	GetWorld()->GetTimerManager().SetTimer(
		SwitchTimerHandle,
		this,
		&URaycastWeaponSwitchAbility::TickTimer,
		0.1f, 
		true  
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		[this, Handle, ActorInfo, ActivationInfo]() {
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		},
		RemainingTime,
		false
	);

}

void URaycastWeaponSwitchAbility::TickTimer()
{
	RemainingTime -= 0.1f;

	GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalHUD()->GetMainHUDWidget()->
		GetGameHUDWidget()->GetWeaponSwitchCooldownWidget()->UpdateCooldownProgress(RemainingTime, SwitchCoolDownDuration);
	
	if (RemainingTime < 0.1f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SwitchTimerHandle);
	}

	// Debug::Print(TEXT("Remaining Time: "), RemainingTime);
}

void URaycastWeaponSwitchAbility::ApplyManaCost()
{
	if (GetCharacterAbilitySystemComponentFromActorInfo() && ManaCostEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = GetCharacterAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			ManaCostEffect,
			1.0f,
			GetCharacterAbilitySystemComponentFromActorInfo()->MakeEffectContext()
		);
		if (SpecHandle.IsValid())
		{
			GetCharacterAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			UE_LOG(LogTemp, Warning, TEXT("Mana başarıyla azaltıldı!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mana Cost Spec oluşturulamadı!"));
		}
	}
}

void URaycastWeaponSwitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ResetTimerHandle) || GetWorld()->GetTimerManager().IsTimerActive(SwitchTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SwitchTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}


