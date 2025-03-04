// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponUIHandler.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/UserInterface/MainHUDWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/SurvivalSystemHUD.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/CurrentWeaponWidget/CurrentWeaponRangedWidgets/ProjectileWeaponWidget/ProjectileCurrentWeaponWidget.h"
#include "Survival/WeaponPickupSystem/UserInterface/GameHUD/GameHUDWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"


UProjectileWeaponUIHandler::UProjectileWeaponUIHandler()
{
	PrimaryComponentTick.bCanEverTick = true;


}


void UProjectileWeaponUIHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

void UProjectileWeaponUIHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProjectileWeaponUIHandler::InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter)
{
	Super::InitializeCallbacks(Weapon, PlayerCharacter);

	if (AProjectileWeapons* ProjectileWeapon = Cast<AProjectileWeapons>(Weapon))
	{
		if (PlayerCharacter)
		{
			if (UProjectileCurrentWeaponWidget* ProjectileCurrentWeaponWidget = PlayerCharacter->GetSurvivalHUD()->GetMainHUDWidget()->GetGameHUDWidget()->GetCurrentWeaponWidget()->GetProjectileCurrentWeaponWidget())
			{
				// TODO: Burada CurrentWeaponWidget çağırıcaz ve RaycastWeapon göndericez! O da diğerlerini Hide edip RaycastUI Visible yapıcak.
				ProjectileCurrentWeaponWidget->BindUICallbacks(ProjectileWeapon);
				ProjectileCurrentWeaponWidget->SetCurrentWeaponImage(ProjectileWeapon->GetProjectileWeaponDataAsset()->WeaponAttributes.GetWeaponImage());
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

void UProjectileWeaponUIHandler::UpdateUI(AWeaponBase* Weapon)
{
	Super::UpdateUI(Weapon);

	if (AProjectileWeapons* ProjectileWeapon = Cast<AProjectileWeapons>(Weapon))
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

void UProjectileWeaponUIHandler::SetupUI(AWeaponBase* Weapon)
{
	Super::SetupUI(Weapon);
}

void UProjectileWeaponUIHandler::RemoveUI(AWeaponBase* Weapon)
{
	Super::RemoveUI(Weapon);

	HideUI.Broadcast();
	if (AProjectileWeapons* ProjectileWeapon = Cast<AProjectileWeapons>(Weapon))
	{
		
	}
	else
	{
		HideUI.Broadcast();
	}
}

