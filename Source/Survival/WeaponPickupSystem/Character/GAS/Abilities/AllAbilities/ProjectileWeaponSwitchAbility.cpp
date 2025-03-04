// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponSwitchAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponInventory/WeaponSwitchWidget/WeaponSwitchCooldownWidget.h"

UProjectileWeaponSwitchAbility::UProjectileWeaponSwitchAbility()
{
	SwitchCoolDownDuration = 2.0f; // it should be same as WeaponSwitchCooldown from CharacterWeaponComponent
	RemainingTime = SwitchCoolDownDuration;
}

void UProjectileWeaponSwitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
			
			if (WeaponInventory->HasWeaponInCategory(EWeaponCategory::Ewc_ProjectileWeapons))
			{
				if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
				{
					WeaponInventory->SwapToBackWeapon(
						CharacterWeaponComponent->GetCurrentWeapon(),
						PlayerCharacter,
						EWeaponCategory::Ewc_ProjectileWeapons
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
				
				if (CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_ProjectileWeapons)
				{
					if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
					{
						WeaponInventory->SwapToBackWeapon(
							CharacterWeaponComponent->GetCurrentWeapon(),
							PlayerCharacter,
							EWeaponCategory::Ewc_ProjectileWeapons
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
		&UProjectileWeaponSwitchAbility::TickTimer,
		0.1f, 
		true  
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		[this, Handle, ActorInfo, ActivationInfo]() {
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		},
		SwitchCoolDownDuration,
		false
	);

}

void UProjectileWeaponSwitchAbility::TickTimer()
{
	RemainingTime -= 0.1f;

	GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalHUD()->GetMainHUDWidget()
		->GetGameHUDWidget()->GetWeaponSwitchCooldownWidget()->UpdateCooldownProgress(RemainingTime, SwitchCoolDownDuration);
	
	if (RemainingTime < 0.1f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SwitchTimerHandle);
	}

	// Debug::Print(TEXT("Remaining Time: "), RemainingTime);
}

void UProjectileWeaponSwitchAbility::ApplyManaCost()
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

void UProjectileWeaponSwitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ResetTimerHandle) || GetWorld()->GetTimerManager().IsTimerActive(SwitchTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SwitchTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ResetTimerHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


// if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
// {
// 	const UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();
// 	
// 	if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
// 	{
// 		PlayerCharacter->GetWeaponInventory()->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), PlayerCharacter, EWeaponCategory::Ewc_ProjectileWeapons);
// 		PlayerCharacter->GetCharacterWeaponComponent()->UpdateLastSwitchTime();
// 	}
// }