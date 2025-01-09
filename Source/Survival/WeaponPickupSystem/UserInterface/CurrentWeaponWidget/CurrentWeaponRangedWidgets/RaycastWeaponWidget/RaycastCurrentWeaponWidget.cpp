// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastCurrentWeaponWidget.h"

#include "Components/WidgetSwitcher.h"
#include "RaycastFocusWidget/RaycastFocusStages/GetOutFromTargetWidget.h"
#include "RaycastFocusWidget/RaycastFocusStages/NoTargetFoundWidget.h"
#include "RaycastFocusWidget/RaycastFocusStages/TargetFoundWidget.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/AmmoWidget/AmmoWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"


void URaycastCurrentWeaponWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void URaycastCurrentWeaponWidget::SetParent(UCurrentWeaponWidget* WeaponWidget)
{
	CurrentWeaponWidgetPtr = WeaponWidget;
}

void URaycastCurrentWeaponWidget::BindUICallbacks(ARaycastWeapons* RaycastWeapon)
{
	RaycastWeaponPtr = RaycastWeapon;
	if (RaycastWeaponPtr.IsValid() && AmmoWidget)
	{
		RaycastWeaponPtr->GetRaycastWeaponUIHandler()->ShowUI.AddDynamic(this, &URaycastCurrentWeaponWidget::ShowCurrentWeaponRaycastWidget);
		RaycastWeaponPtr->GetRaycastWeaponUIHandler()->ShowUI.AddDynamic(this, &URaycastCurrentWeaponWidget::InitializeAmmo);

		ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(RaycastWeapon);
		AmmoWidget->BindUICallbacks(RaycastWeapon);
	}
	
}

void URaycastCurrentWeaponWidget::ShowCurrentWeaponRaycastWidget()
{
	if (!CurrentWeaponWidgetPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponWidgetPtr is invalid or not set!"));
	}

	if (!RaycastWeaponPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RaycastWeaponPtr is invalid or not set!"));
	}
	
	if (CurrentWeaponWidgetPtr.IsValid() && RaycastWeaponPtr.IsValid())
	{
		const auto PlayerCharacter = RaycastWeaponPtr->GetOwningCharacter();
		CurrentWeaponWidgetPtr.Get()->UpdateCurrentWeaponWidget(PlayerCharacter);
	}
}

void URaycastCurrentWeaponWidget::HideCurrentWeaponRaycastWidget()
{
}

void URaycastCurrentWeaponWidget::InitializeAmmo()
{
	if (RaycastWeaponPtr.IsValid())
	{
		const float CurrentAmmo = RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAmmoAttributes.CurrentAmmo;
		const float AmmoCapacity = RaycastWeaponPtr->GetRaycastWeaponDataAsset()->WeaponAmmoAttributes.AmmoCapacity;

		if (AmmoWidget)
		{
			AmmoWidget->UpdateAmmo(CurrentAmmo, AmmoCapacity);
		}
	}
}

void URaycastCurrentWeaponWidget::SetWidgetState(const ETargetWidgetState NewState)
{
    if (!WidgetSwitcher) return;

	switch (NewState)
	{
	case ETargetWidgetState::GetOutFromTarget:
		WidgetSwitcher->SetActiveWidget(GetOutFromTargetWidget);
		break;

	case ETargetWidgetState::TargetFound:
		WidgetSwitcher->SetActiveWidget(TargetFoundWidget);
		break;

	case ETargetWidgetState::NoTargetFound:
		WidgetSwitcher->SetActiveWidget(NoTargetFoundWidget);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Widget State!"));
		break;
	}
}

