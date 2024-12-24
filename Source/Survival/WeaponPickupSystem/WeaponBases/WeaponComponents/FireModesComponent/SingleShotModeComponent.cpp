// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleShotModeComponent.h"

#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


USingleShotModeComponent::USingleShotModeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void USingleShotModeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ARaycastWeapons* Weapon = Cast<ARaycastWeapons>(GetOwner()))
	{
		if (URaycastWeaponData* WeaponData = Cast<URaycastWeaponData>(Weapon->GetWeaponDataAsset()))
		{
			RaycastWeaponData = WeaponData;
			
		}
		OwnerWeapon = Weapon;
	}
}

void USingleShotModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USingleShotModeComponent::Fire()
{
	Super::Fire();
	
	UE_LOG(LogTemp, Warning, TEXT("SingleShotMode Fire called!"));

	if (OwnerWeapon->CanFire())
	{
		OwnerWeapon->SetCanFire(false);
		
		OwnerWeapon->PerformFire();

		if (!GetWorld()) return;
		
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&USingleShotModeComponent::ResetFire,
			RaycastWeaponData->FireRate,
			false
		);
	}
	
}

void USingleShotModeComponent::EndFire()
{
	Super::EndFire();
}

// void USingleShotModeComponent::Fire()
// {
// 
// 	UE_LOG(LogTemp, Warning, TEXT("SingleShotMode Fire called!"));
//
// 	if (OwnerWeapon->CanFire())
// 	{
// 		OwnerWeapon->SetCanFire(false);
// 		
// 		OwnerWeapon->PerformFire();
//
// 		if (!GetWorld()) return;
// 		
// 		GetWorld()->GetTimerManager().SetTimer(
// 			FireRateTimerHandle,
// 			this,
// 			&USingleShotModeComponent::ResetFire,
// 			RaycastWeaponData->FireRate,
// 			false
// 		);
// 	}
//
// }

void USingleShotModeComponent::ResetFire() const
{
	OwnerWeapon->SetCanFire(true);
	UE_LOG(LogTemp, Log, TEXT("Ready to fire again!"));
}

