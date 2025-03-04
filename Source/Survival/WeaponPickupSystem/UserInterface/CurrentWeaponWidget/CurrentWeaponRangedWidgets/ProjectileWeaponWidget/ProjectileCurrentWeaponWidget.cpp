// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileCurrentWeaponWidget.h"

#include "Components/Image.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/AmmoWidget/AmmoWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/ProjectileWeaponUIHandler/ProjectileWeaponUIHandler.h"

void UProjectileCurrentWeaponWidget::SetParent(UCurrentWeaponWidget* WeaponWidget)
{
	CurrentWeaponWidgetPtr = WeaponWidget;
}

void UProjectileCurrentWeaponWidget::BindUICallbacks(AProjectileWeapons* ProjectileWeapon)
{
	ProjectileWeaponPtr = ProjectileWeapon;
	if (ProjectileWeaponPtr.IsValid() && AmmoWidget)
	{
		ProjectileWeaponPtr->GetProjectileWeaponUIHandler()->ShowUI.AddDynamic(this, &UProjectileCurrentWeaponWidget::ShowCurrentWeaponProjectileWidget);
		ProjectileWeaponPtr->GetProjectileWeaponUIHandler()->ShowUI.AddDynamic(this, &UProjectileCurrentWeaponWidget::InitializeAmmo);

		ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(ProjectileWeapon);
		AmmoWidget->BindUICallbacks(ProjectileWeapon);
	}
}

void UProjectileCurrentWeaponWidget::ShowCurrentWeaponProjectileWidget()
{
	if (!CurrentWeaponWidgetPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponWidgetPtr is invalid or not set!"));
	}

	if (!ProjectileWeaponPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RaycastWeaponPtr is invalid or not set!"));
	}
	
	if (CurrentWeaponWidgetPtr.IsValid() && ProjectileWeaponPtr.IsValid())
	{
		const auto PlayerCharacter = ProjectileWeaponPtr->GetOwningCharacter();
		CurrentWeaponWidgetPtr.Get()->UpdateCurrentWeaponWidget(PlayerCharacter);
	}
}

void UProjectileCurrentWeaponWidget::HideCurrentWeaponProjectileWidget()
{
}

void UProjectileCurrentWeaponWidget::SetCurrentWeaponImage(UTexture2D* WeaponImage)
{
	if (CurrentWeaponImage)
	{
		CurrentWeaponImage->SetBrushFromTexture(WeaponImage);
	}
}

void UProjectileCurrentWeaponWidget::InitializeAmmo()
{
	if (ProjectileWeaponPtr.IsValid())
	{
		const float CurrentAmmo = ProjectileWeaponPtr->GetProjectileWeaponDataAsset()->WeaponAmmoAttributes.CurrentAmmo;
		const float AmmoCapacity = ProjectileWeaponPtr->GetProjectileWeaponDataAsset()->WeaponAmmoAttributes.AmmoCapacity;

		if (AmmoWidget)
		{
			AmmoWidget->UpdateAmmo(CurrentAmmo, AmmoCapacity);
		}
	}
}
