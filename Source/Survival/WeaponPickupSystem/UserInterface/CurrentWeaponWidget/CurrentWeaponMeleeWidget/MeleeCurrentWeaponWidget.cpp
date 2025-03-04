// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeCurrentWeaponWidget.h"

#include "Components/Image.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/TemplateWidgets/SurvivalWidgetBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/MeleeWeapons/MeleeWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/MeleeWeaponUIHandler/MeleeWeaponUIHandler.h"


void UMeleeCurrentWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Debug::Print(TEXT("Rage Widget Created"));
}

void UMeleeCurrentWeaponWidget::SetParent(UCurrentWeaponWidget* WeaponWidget)
{
	CurrentWeaponWidgetPtr = WeaponWidget;
}

void UMeleeCurrentWeaponWidget::BindUICallbacks(AMeleeWeapon* MeleeWeapon)
{
	MeleeWeaponPtr = MeleeWeapon;
	if (MeleeWeaponPtr.IsValid())
	{
		MeleeWeaponPtr->GetMeleeWeaponUIHandler()->ShowUI.AddDynamic(this, &UMeleeCurrentWeaponWidget::ShowCurrentWeaponMeleeWidget);
	}
}

void UMeleeCurrentWeaponWidget::ShowCurrentWeaponMeleeWidget()
{
	if (!CurrentWeaponWidgetPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponWidgetPtr is invalid or not set!"));
	}

	if (!MeleeWeaponPtr.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("RaycastWeaponPtr is invalid or not set!"));
	}
	
	if (CurrentWeaponWidgetPtr.IsValid() && MeleeWeaponPtr.IsValid())
	{
		const auto PlayerCharacter = MeleeWeaponPtr->GetOwningCharacter();
		CurrentWeaponWidgetPtr.Get()->UpdateCurrentWeaponWidget(PlayerCharacter);
	}
}

void UMeleeCurrentWeaponWidget::HideCurrentWeaponMeleeWidget()
{
}

void UMeleeCurrentWeaponWidget::SetCurrentWeaponImage(UTexture2D* WeaponImage)
{
	if (CurrentWeaponImage)
	{
		CurrentWeaponImage->SetBrushFromTexture(WeaponImage);
	}
}

void UMeleeCurrentWeaponWidget::UpdateRageBar(float NewRage, float MaxRage)
{
	if (RageBar)
	{
		RageBar->SetStatusBarPercent(NewRage / MaxRage);
	}
}

void UMeleeCurrentWeaponWidget::SetRageBarVisibility(bool SetVisibility)
{
	if (RageBar)
	{
		if (SetVisibility)
		{
			RageBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			RageBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
