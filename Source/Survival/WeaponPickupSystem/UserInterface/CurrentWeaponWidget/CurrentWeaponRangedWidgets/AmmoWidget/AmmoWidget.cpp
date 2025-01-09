// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoWidget.h"

#include "Components/TextBlock.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"

void UAmmoWidget::BindUICallbacks(ARangedWeapon* RangedWeapon)
{
	RangedWeapon->OnAmmoChange.AddDynamic(this, &UAmmoWidget::UpdateAmmo);
}

void UAmmoWidget::UpdateAmmo(const int32 CurrentAmmo, const int32 AmmoCapacity)
{
	SetCurrentAmmo(CurrentAmmo);
	SetTotalAmmo(AmmoCapacity);
}

void UAmmoWidget::SetCurrentAmmo(const int32 Ammo) const
{
	CurrentAmmoText->SetText(FText::AsNumber(Ammo));
}

void UAmmoWidget::SetTotalAmmo(const int32 Ammo) const
{
	TotalAmmoText->SetText(FText::AsNumber(Ammo));
}
