// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticShotModeComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"


UAutomaticShotModeComponent::UAutomaticShotModeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAutomaticShotModeComponent::BeginPlay()
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

void UAutomaticShotModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAutomaticShotModeComponent::Fire()
{
	Super::Fire();

	if (OwnerWeapon->CanFire())
	{
		OwnerWeapon->SetCanFire(false);

		OwnerWeapon->PerformFire();

		if (!GetWorld()) return;

		GetWorld()->GetTimerManager().SetTimer(
			AutomaticFireTimerHandle,
			this,
			&UAutomaticShotModeComponent::AutomaticFire,
			RaycastWeaponData->FireRate,
			true
		);
	}
	
}

void UAutomaticShotModeComponent::EndFire()
{
	Super::EndFire();
	
	if (GetWorld() && !OwnerWeapon->CanFire() && !OwnerWeapon->GetAttackCooldownActive())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
		
		OwnerWeapon->SetAttackCooldownActive(true);
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle,
			this,
			&UAutomaticShotModeComponent::ResetFire,
			RaycastWeaponData->FireRate,
			false
		);
	}

}

void UAutomaticShotModeComponent::AutomaticFire()
{
	if (OwnerWeapon)
	{
		OwnerWeapon->SetCanFire(false);
		OwnerWeapon->PerformFire();
	}

	// if (OwnerWeapon->GetOwningCharacter()->GetAbilitySystemComponent()->TryActivateAbilityByClass())
	// {
	// 	 TODO: Silahın RangedWeapon kısmına Fire Ability koyarsak. Bunu AutomaticFire üzerinden sürekli ateş edebiliriz. Loopa alıp çağırabiliriz.
	//		Ability her çağrıldığında tekrar Attack çağrılacak. Bunun önüne geçmek için bool koy belkide tag koyabilirsin. üzerinde düşün!
	//		Tag koyarsak automatic atış modunda olduğumuz bilinir ve modlara göre bir ayar yapabiliriz.
	// }
}

void UAutomaticShotModeComponent::ResetFire() const
{
	OwnerWeapon->SetCanFire(true);
	OwnerWeapon->SetAttackCooldownActive(false);
	UE_LOG(LogTemp, Log, TEXT("Ready to fire again!"));
}

