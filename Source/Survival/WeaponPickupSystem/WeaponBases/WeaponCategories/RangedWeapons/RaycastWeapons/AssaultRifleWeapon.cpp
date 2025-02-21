// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifleWeapon.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/PickupSystem/BasePickup.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/AutomaticShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/BurstShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"


// Sets default values
AAssaultRifleWeapon::AAssaultRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("AAssaultRifleWeapon constructor called"));

	BurstShotModeComponent = CreateDefaultSubobject<UBurstShotModeComponent>(TEXT("BurstShotModeComponent"));
	AutomaticShotModeComponent = CreateDefaultSubobject<UAutomaticShotModeComponent>(TEXT("AutomaticShotModeComponent"));
}


void AAssaultRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAssaultRifleWeapon::AddFireModes()
{
	Super::AddFireModes();

	FireModeComponents.Add(BurstShotModeComponent);
	FireModeComponents.Add(AutomaticShotModeComponent);
	

	if (FireModeComponents.Num() > 0)
	{
		CurrentFireModeIndex = 0;
		CurrentFireModeComponent = FireModeComponents[CurrentFireModeIndex];
		UE_LOG(LogTemp, Log, TEXT("Initialized fire mode: %s"), *CurrentFireModeComponent->GetClass()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No fire modes found on %s"), *GetName());
	}
	
}

void AAssaultRifleWeapon::PerformFire()
{
	Super::PerformFire();

	if (GetHeatComponent()->GetCurrentHeat() >= GetRaycastWeaponDataAsset()->FiringHeatSettings.MaxHeatCapacity || GetHeatComponent()->IsOverHeated() || GetCurrentAmmo() <= 0) { return; }
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	OnFireMade.Broadcast();
	// OnFirePlayMontage.Broadcast();
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		RaycastMuzzleLocation,
		RaycastTraceEnd,
		ECC_Visibility,
		QueryParams
		);

	if (bHit)
	{
		OnImpact.Broadcast(HitResult);

		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->ActorHasTag(FName("Enemy")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy hit! Applying damage..."));

				// OnRayHitTarget.ExecuteIfBound(HitActor);
				OnRayHitTarget.Broadcast(HitActor);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast missed. No hit detected."));
	}
	
	DrawDebugVisuals(RaycastMuzzleLocation, RaycastTraceEnd, HitResult);
}

