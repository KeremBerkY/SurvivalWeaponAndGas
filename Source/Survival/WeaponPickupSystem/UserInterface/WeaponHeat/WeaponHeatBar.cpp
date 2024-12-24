// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHeatBar.h"

#include "Components/ProgressBar.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

void UWeaponHeatBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HeatProgressBar->SetPercent(0.f);
}


void UWeaponHeatBar::BindUICallbacks(ARaycastWeapons* RaycastWeapon)
{
	RaycastWeapon->GetRaycastWeaponUIHandler()->ShowUI.AddDynamic(this, &UWeaponHeatBar::ShowWidget); 
	RaycastWeapon->GetRaycastWeaponUIHandler()->HideUI.AddDynamic(this, &UWeaponHeatBar::HideWidget);
	RaycastWeapon->GetRaycastWeaponUIHandler()->OnUpdateHeatBar.AddDynamic(this, &UWeaponHeatBar::UpdateHeatBar);
}

void UWeaponHeatBar::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UWeaponHeatBar::HideWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void UWeaponHeatBar::UpdateHeatBar(float Heat, float MaxHeat)
	{
	if (HeatProgressBar)
	{
		float HeatPercentage = FMath::Clamp(Heat / MaxHeat, 0.0f, 1.0f);
		HeatProgressBar->SetPercent(HeatPercentage);
	}
}

// void UWeaponHeatBar::HalfHeat()
// {
// 	UStaticMeshComponent* WeaponMesh = Weapon->GetWeaponMesh();
// 	if (WeaponMesh)
// 	{
// 		WeaponMesh->SetRenderCustomDepth(true);
// 	}
// }
//
// void UWeaponHeatBar::OverHeat()
// {
// 	UStaticMeshComponent* WeaponMesh = Weapon->GetWeaponMesh();
// 	if (WeaponMesh)
// 	{
// 		WeaponMesh->SetRenderCustomDepth(false);
// 	}
// }

// void UWeaponHeatBar::RemoveHeat(float Heat, float MaxHeat)
// {
// }
