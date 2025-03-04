// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponUIHandler.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponMeleeWidget/MeleeCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/ResourceWidget/ResourceWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/MeleeWeapons/MeleeWeapon.h"


UMeleeWeaponUIHandler::UMeleeWeaponUIHandler()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMeleeWeaponUIHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMeleeWeaponUIHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMeleeWeaponUIHandler::InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter)
{
	Super::InitializeCallbacks(Weapon, PlayerCharacter);

	if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(Weapon))
	{
		if (PlayerCharacter)
		{
			if (const auto GameHUDWidget = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget())
			{
				if (UMeleeCurrentWeaponWidget* MeleeCurrentWeaponWidget = GameHUDWidget->GetCurrentWeaponWidget()->GetMeleeCurrentWeaponWidget())
				{
					// TODO: Burada CurrentWeaponWidget çağırıcaz ve RaycastWeapon göndericez! O da diğerlerini Hide edip RaycastUI Visible yapıcak.
					MeleeCurrentWeaponWidget->BindUICallbacks(MeleeWeapon);
					MeleeCurrentWeaponWidget->SetCurrentWeaponImage(MeleeWeapon->GetWeaponDataAsset()->WeaponAttributes.GetWeaponImage());
					MeleeCurrentWeaponWidget->SetRageBarVisibility(true);
					
					// OnInitializeCallbacks.Broadcast(Weapon);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("There is no HeatProgressBar!!!"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("There is no PlayerCharacter reference"));
		}
	}
}

void UMeleeWeaponUIHandler::UpdateUI(AWeaponBase* Weapon)
{
	Super::UpdateUI(Weapon);

	if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(Weapon))
	{
		if (ShowUI.IsBound())
		{
			ShowUI.Broadcast();
			// TODO burada show çalışacak.
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ShowUI delegate is not bound!"));
		}
	}
	else
	{
		HideUI.Broadcast();
	}
}

void UMeleeWeaponUIHandler::SetupUI(AWeaponBase* Weapon)
{
	Super::SetupUI(Weapon);
}

void UMeleeWeaponUIHandler::RemoveUI(AWeaponBase* Weapon)
{
	Super::RemoveUI(Weapon);

	HideUI.Broadcast();
	if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(Weapon))
	{
		
	}
	else
	{
		HideUI.Broadcast();
	}
}

