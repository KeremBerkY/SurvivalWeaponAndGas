// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleFireModeAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/WeaponFireModesWidget/WeaponFireModesWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/FireModeBaseComponent.h"

UToggleFireModeAbility::UToggleFireModeAbility()
{
	bIsCooldownActive = false;
}

void UToggleFireModeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bIsCooldownActive)
	{
		if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (ARaycastWeapons* CurrentWeapon = Cast<ARaycastWeapons>(PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon()))
			{
				bIsCooldownActive = true;
				
				const auto FireModeComponents = CurrentWeapon->GetFireModeComponents();
				if (FireModeComponents.Num() <= 1)
				{
					UE_LOG(LogTemp, Warning, TEXT("Only one or no fire mode available."));
					return;
				}

				const int32 CurrentFireModeIndex = (CurrentWeapon->GetCurrentFireModeIndex() + 1) % FireModeComponents.Num();
				CurrentWeapon->SetCurrentFireModeIndex(CurrentFireModeIndex);
				const auto CurrentFireModeComponent = FireModeComponents[CurrentFireModeIndex];
				CurrentWeapon->SetCurrentFireModeComponent(CurrentFireModeComponent);	
				
				if (CurrentFireModeComponent)
				{
					if (FireModeComponents.Num() > 1)
					{
						if (UWeaponFireModesWidget* WeaponFireModesWidget = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()
							->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget()->GetWeaponFireModesWidget())
						{
							WeaponFireModesWidget->SetFireModeState(CurrentFireModeComponent->GetFireModeType()); // TODO: Bu yapı yanlış oldu düzelt!! idex gönderiyosun eğer single ve automatic add ederse birisi gönderdiğin 1 indexi burst olacak ama sen automatic seçmiş oluyorsun.
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to switch fire mode. CurrentFireModeComponent is nullptr."));
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("There is no CurrentWeapon."));
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	
	FTimerHandle CooldownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UToggleFireModeAbility::Cooldown, 1.f, false);
}

void UToggleFireModeAbility::Cooldown()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UToggleFireModeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	bIsCooldownActive = false;
}
