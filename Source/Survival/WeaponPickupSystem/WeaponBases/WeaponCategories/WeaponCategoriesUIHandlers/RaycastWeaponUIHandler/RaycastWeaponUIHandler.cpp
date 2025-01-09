// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeaponUIHandler.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/Libraries/CustomDepthHelper.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/WeaponHeat/WeaponHeatBar.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

URaycastWeaponUIHandler::URaycastWeaponUIHandler()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URaycastWeaponUIHandler::BeginPlay()
{
	Super::BeginPlay();
	
}


void URaycastWeaponUIHandler::InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter)
{
	IWeaponUIHandler::InitializeCallbacks(Weapon, PlayerCharacter);

	if (ARaycastWeapons* RaycastWeapon = Cast<ARaycastWeapons>(Weapon))
	{
		if (PlayerCharacter)
		{
			UWeaponHeatBar* HeatProgressBar = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetHeatProgressBar();
			URaycastCurrentWeaponWidget* RaycastCurrentWeaponWidget = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetRaycastCurrentWeaponWidget();
			if (HeatProgressBar && RaycastCurrentWeaponWidget)
			{
				HeatProgressBar->BindUICallbacks(RaycastWeapon);
				// TODO: Burada CurrentWeaponWidget çağırıcaz ve RaycastWeapon göndericez! O da diğerlerini Hide edip RaycastUI Visible yapıcak.
				RaycastCurrentWeaponWidget->BindUICallbacks(RaycastWeapon);
				// OnInitializeCallbacks.Broadcast(Weapon);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("There is no HeatProgressBar!!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("There is no PlayerCharacter reference"));
		}
	}
	
}

void URaycastWeaponUIHandler::UpdateUI(AWeaponBase* Weapon)
{
	IWeaponUIHandler::UpdateUI(Weapon);
	// UCurrentWeaponWidget* CurrentWeaponWidget = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget();

	
	if (ARaycastWeapons* RaycastWeapon = Cast<ARaycastWeapons>(Weapon))
	{
		// RaycastWeapon->GetHeatComponent()->UpdateHeatBar.Broadcast(0.f, RaycastWeapon->GetRaycastWeaponDataAsset()->FiringHeatSettings.MaxHeatCapacity);
		OnUpdateHeatBar.Broadcast(0.f,RaycastWeapon->GetRaycastWeaponDataAsset()->FiringHeatSettings.MaxHeatCapacity);
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

void URaycastWeaponUIHandler::SetupUI(AWeaponBase* Weapon)
{
	IWeaponUIHandler::SetupUI(Weapon);

}

void URaycastWeaponUIHandler::RemoveUI(AWeaponBase* Weapon)
{
	IWeaponUIHandler::RemoveUI(Weapon);
	
	HideUI.Broadcast();
	if (ARaycastWeapons* RaycastWeapon = Cast<ARaycastWeapons>(Weapon))
	{
		RaycastWeapon->GetHeatComponent()->SetCurrentHeat(0.f);
		RaycastWeapon->GetHeatComponent()->ClearHeatCoolerTimer();
	}
	else
	{
		HideUI.Broadcast();
	}

}


void URaycastWeaponUIHandler::UpdateHeatBar(float Heat, float MaxHeat)
{
	OnUpdateHeatBar.Broadcast(Heat, MaxHeat);
}


