// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentWeaponWidget.h"

#include "Components/WidgetSwitcher.h"
#include "CurrentWeaponRangedWidgets/RaycastWeaponWidget/RaycastCurrentWeaponWidget.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"

void UCurrentWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RaycastCurrentWeapon)
	{
		RaycastCurrentWeapon->SetParent(this);
	}

	SetVisibility(ESlateVisibility::Hidden);
}

void UCurrentWeaponWidget::UpdateCurrentWeaponWidget(const ASurvivalCharacter* PlayerCharacter)
{
	if (!WidgetSwitcher) return;

	HideCurrentWeaponWidget();
	
	if (const auto CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		SetVisibility(ESlateVisibility::Visible);
		auto WeaponCategory = CurrentWeapon->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory;

		switch (WeaponCategory)
		{
		case EWeaponCategory::Ewc_RaycastWeapons:
			WidgetSwitcher->SetActiveWidget(RaycastCurrentWeapon);
			// TODO: Burada callbackleri bind et raycast için.
			break;
			
		case EWeaponCategory::Ewc_ProjectileWeapons:
			// WidgetSwitcher->SetActiveWidget(ProjectileCurrentWeapon);
			break;
			
		case EWeaponCategory::Ewc_MeleeWeapons:
			// WidgetSwitcher->SetActiveWidget(MeleeCurrentWeapon);
			break;
		case EWeaponCategory::Ewc_Max:
			break;
		}
	}
	else
	{
		HideCurrentWeaponWidget();
	}
}

void UCurrentWeaponWidget::HideCurrentWeaponWidget()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(-1);
	}
	SetVisibility(ESlateVisibility::Hidden);
}


