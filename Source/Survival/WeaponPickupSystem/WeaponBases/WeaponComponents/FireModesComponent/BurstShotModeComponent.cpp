// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstShotModeComponent.h"

#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"


UBurstShotModeComponent::UBurstShotModeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BurstShotRemaining = 3;
	BurstCount = 3;
}

void UBurstShotModeComponent::BeginPlay()
{
	Super::BeginPlay();

	RaycastWeapon = Cast<ARaycastWeapons>(GetOwner());
	if (RaycastWeapon)
	{
		if (URaycastWeaponData* WeaponData = Cast<URaycastWeaponData>(RaycastWeapon->GetWeaponDataAsset()))
		{
			RaycastWeaponData = WeaponData;
			
		}
		OwnerWeapon = RaycastWeapon;
	}

	RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity;
	
}

void UBurstShotModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBurstShotModeComponent::Fire()
{
	Super::Fire();
	
	if (OwnerWeapon->CanFire())
	{
		OwnerWeapon->SetCanFire(false);

		OwnerWeapon->PerformFire();
		BurstShotRemaining--;

		if (!GetWorld()) return;

		GetWorld()->GetTimerManager().SetTimer(
			BurstFireTimerHandle,
			this,
			&UBurstShotModeComponent::BurstFire,
			RaycastWeaponData->FireRate,
			true
		);
	}
	
}

void UBurstShotModeComponent::EndFire()
{
	Super::EndFire();

	if (GetWorld() && !OwnerWeapon->GetAttackCooldownActive() && !OwnerWeapon->CanFire() ) //  && !OwnerWeapon->GetAttackCooldownActive() &&  && !OwnerWeapon->CanFire()
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);

		
		OwnerWeapon->SetAttackCooldownActive(true);
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&UBurstShotModeComponent::ResetFire,
			RaycastWeaponData->FireRate,
			false
		);
	}
}

void UBurstShotModeComponent::BurstFire()
{
	if (OwnerWeapon && BurstShotRemaining > 0 && !RaycastWeapon->GetHeatComponent()->IsOverHeated())
	{
		BurstShotRemaining--;
		OwnerWeapon->PerformFire();
	}
	else
	{
		if (!RaycastWeapon->GetHeatComponent()->IsOverHeated())
		{
			BurstShotRemaining = BurstCount;
			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					BurstCoolDownTimerHandle,
					this,
					&UBurstShotModeComponent::BurstFireCooldown,
					RaycastWeaponData->FireRate / 2.f,
					false
				);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
			OwnerWeapon->SetCanFire(true);
			OwnerWeapon->SetAttackCooldownActive(false);
		}
	}
}

void UBurstShotModeComponent::ResetFire() const
{
	OwnerWeapon->SetCanFire(true);
	OwnerWeapon->SetAttackCooldownActive(false);
	UE_LOG(LogTemp, Log, TEXT("Ready to fire again!"));
}

void UBurstShotModeComponent::BurstFireCooldown()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			BurstFireTimerHandle,
			this,
			&UBurstShotModeComponent::BurstFire,
			RaycastWeaponData->FireRate,
			true
		);
	}
}


