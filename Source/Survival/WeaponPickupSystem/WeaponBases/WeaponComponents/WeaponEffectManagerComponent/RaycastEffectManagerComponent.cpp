// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastEffectManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Components/DecalComponent.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"


URaycastEffectManagerComponent::URaycastEffectManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URaycastEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponPtr = MakeWeakObjectPtr(Cast<ARaycastWeapons>(GetOwner()));
	if (WeaponPtr.IsValid())
	{
		WeaponPtr->OnInitializedComponents.AddDynamic(this, &URaycastEffectManagerComponent::InitializeEffects);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Weapon! Weapon = nullptr."));
	}

}

void URaycastEffectManagerComponent::InitializeEffects()
{
	RaycastWeaponDataPtr = MakeWeakObjectPtr(WeaponPtr->GetRaycastWeaponDataAsset());
	
	if (!RaycastWeaponDataPtr.IsValid()) return;
	WeaponPtr->OnFireMade.AddDynamic(this, &URaycastEffectManagerComponent::PlayWeaponEffects);
	WeaponPtr->OnImpact.AddDynamic(this, &URaycastEffectManagerComponent::PlayImpactEffects);
}

void URaycastEffectManagerComponent::PlayWeaponEffects()
{
	// if (Weapon->GetHeatComponent()->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || Weapon->GetHeatComponent()->IsOverHeated() ) { return; }
	
	WeaponEffect();
	if (WeaponPtr->GetWeaponDataAsset()->WeaponAttributes.WeaponTypes != EWeaponTypes::Ewt_Shotgun)
	{
		ApplyRecoilEffect();
	}
}

void URaycastEffectManagerComponent::PlayImpactEffects(const FHitResult& HitResult)
{
	// if (Weapon->GetHeatComponent()->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || Weapon->GetHeatComponent()->IsOverHeated() ) { return; }

	ImpactEffect(HitResult);
	ApplyDecal(HitResult);
}

void URaycastEffectManagerComponent::WeaponEffect() const
{
	
	USceneComponent* MuzzleComponent = WeaponPtr->GetMuzzleLocation();
	
	if (!MuzzleComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleComponent is null. Effects cannot be triggered."));
		return;
	}
	
	if (RaycastWeaponDataPtr.Get()->WeaponEffects.MuzzleFlashEffect)
	{
		FVector EffectLocation = MuzzleComponent->GetComponentLocation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaycastWeaponDataPtr->WeaponEffects.MuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no MuzzleFlashEffect."));
	}
	
}

void URaycastEffectManagerComponent::ApplyRecoilEffect() const
{
	if (!WeaponPtr.Get()->GetWeaponMesh()) return;
	
	float RecoilAmount = RaycastWeaponDataPtr->RecoilSettings.BaseRecoilAmount + FMath::FRandRange(
		-RaycastWeaponDataPtr->RecoilSettings.RandomRecoilRange,
		RaycastWeaponDataPtr->RecoilSettings.RandomRecoilRange
		);
	
	FVector RecoilOffset = FVector(
		-RecoilAmount * RaycastWeaponDataPtr->RecoilSettings.PitchMultiplier,
		RecoilAmount * RaycastWeaponDataPtr->RecoilSettings.YawMultiplier,
		0.0f
		);

	FVector OriginalLocation = WeaponPtr->GetWeaponMesh()->GetRelativeLocation();
	WeaponPtr->GetWeaponMesh()->SetRelativeLocation(OriginalLocation + RecoilOffset);
	
	ResetWeaponPosition(OriginalLocation);
}


void URaycastEffectManagerComponent::ResetWeaponPosition(FVector OriginalLocation) const
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, OriginalLocation]()
	{
		if (WeaponPtr->GetWeaponMesh())
		{
			WeaponPtr->GetWeaponMesh()->SetRelativeLocation(OriginalLocation);
		}
	}, 0.1f, false); 
}

void URaycastEffectManagerComponent::ImpactEffect(const FHitResult& HitResult) const
{
	if (HitResult.GetActor()->IsA(AEnemyBase::StaticClass()))
	{
		if (RaycastWeaponDataPtr.Get()->BloodEffect)
		{
			const FVector ImpactLocation = HitResult.ImpactPoint;
			const FRotator ImpactRotation = HitResult.ImpactNormal.Rotation();
		
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaycastWeaponDataPtr->BloodEffect, ImpactLocation, ImpactRotation);
		}
	}
	else
	{
		if (RaycastWeaponDataPtr.Get()->WeaponEffects.ImpactEffect)
		{
			const FVector ImpactLocation = HitResult.ImpactPoint;
			const FRotator ImpactRotation = HitResult.ImpactNormal.Rotation();
		
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaycastWeaponDataPtr->WeaponEffects.ImpactEffect, ImpactLocation, ImpactRotation);
		}
	}
}

void URaycastEffectManagerComponent::ApplyDecal(const FHitResult& HitResult) const
{

	if (RaycastWeaponDataPtr.Get()->DecalSettings.DecalMaterial)
	{
		if (UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			RaycastWeaponDataPtr->DecalSettings.DecalMaterial,
			RaycastWeaponDataPtr->DecalSettings.DecalSize,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation(),
			RaycastWeaponDataPtr->DecalSettings.DecalLifeSpan
		))
		{
			Decal->SetFadeOut(RaycastWeaponDataPtr->DecalSettings.DecalLifeSpan, 2.0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Decal spawn failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Decal Material assigned."));
	}
}
