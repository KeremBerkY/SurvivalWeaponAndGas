// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/Libraries/CustomDepthHelper.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"

UHeatComponent::UHeatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHeat = 0;
	bIsCoolingDown = false;
	bIsOverHeated = false;
	LastFiredTime = 0.f;

}

void UHeatComponent::BeginPlay()
{
	Super::BeginPlay();

	Weapon = Cast<ARaycastWeapons>(GetOwner());
	if (Weapon)
	{
		Weapon->OnInitializedComponents.AddDynamic(this, &UHeatComponent::InitializeHeat);
	}
	
}

void UHeatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	LastFiredTime = (LastFiredTime >= 10) ? LastFiredTime : LastFiredTime + DeltaTime;
	
	if (CanStartCooling() && !bIsCoolingDown && !CompleteCooling())
	{
		StartCooling();
	}

	if (CurrentHeat == 0)
	{
		Weapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0);
	}
}

void UHeatComponent::InitializeHeat()
{
	WeaponDataAsset = Weapon->GetRaycastWeaponDataAsset();

	if (!WeaponDataAsset) return;
	
	if (!Weapon->UpdateHeat.IsAlreadyBound(this, &UHeatComponent::UpdateHeat))
	{
		Weapon->UpdateHeat.AddDynamic(this, &UHeatComponent::UpdateHeat);
	}

}

void UHeatComponent::UpdateHeat()
{
	if (!Weapon || !WeaponDataAsset) return;

	if (CurrentHeat < WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity)
	{
		const float HeatToAdd = WeaponDataAsset->FiringHeatSettings.HeatGeneratedPerShot;
		CurrentHeat = FMath::Clamp(CurrentHeat + HeatToAdd, 0.0f, WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity);
		
		Weapon->GetRaycastWeaponUIHandler()->UpdateHeatBar(CurrentHeat, WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity);
		ResetHeatState();

		if (IsOverheated())
		{
			Weapon->GetWeaponMesh()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1);
			bIsOverHeated = true;
			StartCooling();
		}
	}
}

bool UHeatComponent::CanStartCooling() const
{
	if (!WeaponDataAsset) return false;
	return LastFiredTime > WeaponDataAsset->FiringHeatSettings.HeatCooldownRate;
}

bool UHeatComponent::IsOverheated() const
{
	return CurrentHeat >= WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity;
}

bool UHeatComponent::CompleteCooling() const
{
	return CurrentHeat <= 0;
}

void UHeatComponent::StartCooling()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(CoolingTimerHandle))
	{
		bIsCoolingDown = true;
		GetWorld()->GetTimerManager().SetTimer(CoolingTimerHandle, this, &UHeatComponent::ApplyCooling, 1.0f, true);
	}
}

void UHeatComponent::ApplyCooling()
{
	const float AddCooler = (WeaponDataAsset->FiringHeatSettings.HeatGeneratedPerShot * 2);
	CurrentHeat = FMath::Clamp(CurrentHeat - AddCooler, 0, WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity);

	// if (CurrentHeat <= WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity * 0.6f)
	// {
	// 	UCustomDepthHelper::ResetCustomDepth(Weapon->GetWeaponMesh());
	//
	// }
	
	if (CurrentHeat <= 0.f)
	{
		CurrentHeat = 0.f;
		bIsOverHeated = false;
		GetWorld()->GetTimerManager().ClearTimer(CoolingTimerHandle);
	}
	
	Weapon->GetRaycastWeaponUIHandler()->UpdateHeatBar(CurrentHeat, WeaponDataAsset->FiringHeatSettings.MaxHeatCapacity);
}

void UHeatComponent::ClearHeatCoolerTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CoolingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolingTimerHandle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cooling Timer is not active."));
	}
}


void UHeatComponent::ResetHeatState()
{
	bIsCoolingDown = false;
	GetWorld()->GetTimerManager().ClearTimer(CoolingTimerHandle);
	LastFiredTime = 0.f;
}

