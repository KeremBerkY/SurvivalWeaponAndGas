// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

URaycastAttackAbility::URaycastAttackAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Raycast"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(AbilityTag);
	
}

//TODO: sol tuşa basılı tuttun loop döndü sonra parmağını çekip tekrar basarsan end çalışmıyor. bu sınıf için
void URaycastAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const auto WeaponRef = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	if (PlayerCharacterRef && WeaponRef)
	{
		RaycastWeaponPtr = MakeWeakObjectPtr(Cast<ARaycastWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()));
		const auto RaycastWeaponData = RaycastWeaponPtr->GetRaycastWeaponDataAsset();

		if (!RaycastWeaponData)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
		
		if (RaycastWeaponPtr->GetAttackCooldownActive())
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}

		if (RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAnimMontages.bPlayFromMontage && !RaycastWeaponPtr->CanFire()) //  RaycastWeaponPtr->GetCurrentFireModeIndex() ile bir kontrol sağlayabilirsin?
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}
		
		if (!RaycastWeaponPtr.IsValid() || RaycastWeaponPtr->GetCurrentAmmo() <= 0 ||
			RaycastWeaponPtr->GetHeatComponent()->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity ||
			RaycastWeaponPtr->GetHeatComponent()->IsOverHeated())
		{
			UE_LOG(LogTemp, Warning, TEXT("EndAbility Called Before Montage!!!"));
			WeaponRef->EndAttack();
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		};
		
		AbilityMontage = RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAnimMontages.FireMontage;
		MontageRate = 1 + (RaycastWeaponPtr->GetRaycastWeaponDataAsset()->FireRate);
	
		StartAnimMontage();
		PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->Attack();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has no Weapon"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	if (!RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAnimMontages.bPlayFromMontage)
	{
		if (!GetCharacterAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Player.Weapon.FireMode.Automatic"))) &&
			!GetCharacterAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Player.Weapon.FireMode.Burst"))))
		{
			UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
			WaitInputReleaseTask->OnRelease.AddDynamic(this, &URaycastAttackAbility::OnInputRelease);
			WaitInputReleaseTask->Activate();
			Debug::Print(TEXT("WaitInputReleaseTask bind"));
		}
	}
}
 
void URaycastAttackAbility::OnInputRelease(float TimeHeld)
{
	// TODO: AutomaticShot da ateş etmeye devam ediyor. Bunu Kaldırmamız lazım! (Shot Modelarına Tag ekleyelim. Burada da Tagler check edilsin.
	if (PlayerCharacterRef)
	{
		// !CurrentWeapon->GetAttackCooldownActive
		const auto CurrentWeapon = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->EndAttack();
			Debug::Print("EndAbility Called!", FColor::Orange);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}

void URaycastAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		HandleApplyDamage(Payload);
	}
}

void URaycastAttackAbility::HandleApplyDamage(const FGameplayEventData& GameplayEventData)
{
	if (AActor* LocalTargetActor = Cast<AActor>(GameplayEventData.Target))
	{
		if (USurvivalCharacterCombatComponent* SurvivalCharacterCombatComponent = GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalCharacterCombatComponent())
		{
			const float WeaponBaseDamage = SurvivalCharacterCombatComponent->GetCharacterCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
			const auto SpecHandle = MakeCharacterDamageEffectSpecHandle(
				DamageEffect,
				WeaponBaseDamage,
				FGameplayTag::RequestGameplayTag(FName("Character.SetByCaller.AttackType.Ray"))
			);
			NativeApplyEffectSpecHandleToTarget(LocalTargetActor, SpecHandle);
		}
	}
}

void URaycastAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);
	
	if (PlayerCharacterRef)
	{
		// !CurrentWeapon->GetAttackCooldownActive
		const auto CurrentWeapon = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->EndAttack();
			Debug::Print("EndAbility Called!", FColor::Orange);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);
	
	if (RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAnimMontages.bPlayFromMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}


void URaycastAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}
