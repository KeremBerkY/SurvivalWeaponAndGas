// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastEffectManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Components/DecalComponent.h"



URaycastEffectManagerComponent::URaycastEffectManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URaycastEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Weapon = Cast<ARaycastWeapons>(GetOwner());
	if (Weapon)
	{
		Weapon->OnInitializedComponents.AddDynamic(this, &URaycastEffectManagerComponent::InitializeEffects);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Weapon! Weapon = nullptr."));
	}

}

void URaycastEffectManagerComponent::InitializeEffects()
{
	RaycastWeaponData = Weapon->GetRaycastWeaponDataAsset();
	
	if (!RaycastWeaponData) return;
	Weapon->OnFireMade.AddDynamic(this, &URaycastEffectManagerComponent::PlayWeaponEffects);
	Weapon->OnImpact.AddDynamic(this, &URaycastEffectManagerComponent::PlayImpactEffects);
}

void URaycastEffectManagerComponent::PlayWeaponEffects()
{
	WeaponEffect();
	// ApplyRecoilEffect();
}

void URaycastEffectManagerComponent::PlayImpactEffects(const FHitResult& HitResult)
{
	ImpactEffect(HitResult);
	ApplyDecal(HitResult);
}

void URaycastEffectManagerComponent::WeaponEffect() const
{
	
	USceneComponent* MuzzleComponent = Weapon->GetMuzzleLocation();
	
	if (!MuzzleComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleComponent is null. Effects cannot be triggered."));
		return;
	}
	
	if (RaycastWeaponData->WeaponEffects.MuzzleFlashEffect)
	{
		FVector EffectLocation = MuzzleComponent->GetComponentLocation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaycastWeaponData->WeaponEffects.MuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no MuzzleFlashEffect."));
	}
	
}

void URaycastEffectManagerComponent::ApplyRecoilEffect() const
{
	if (!Weapon->GetWeaponMesh()) return;
	
	float RecoilAmount = RaycastWeaponData->RecoilSettings.BaseRecoilAmount + FMath::FRandRange(
		-RaycastWeaponData->RecoilSettings.RandomRecoilRange,
		RaycastWeaponData->RecoilSettings.RandomRecoilRange
		);
	
	FVector RecoilOffset = FVector(
		-RecoilAmount * RaycastWeaponData->RecoilSettings.PitchMultiplier,
		RecoilAmount * RaycastWeaponData->RecoilSettings.YawMultiplier,
		0.0f
		);

	FVector OriginalLocation = Weapon->GetWeaponMesh()->GetRelativeLocation();
	Weapon->GetWeaponMesh()->SetRelativeLocation(OriginalLocation + RecoilOffset);
	
	ResetWeaponPosition(OriginalLocation);
}


void URaycastEffectManagerComponent::ResetWeaponPosition(FVector OriginalLocation) const
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, OriginalLocation]()
	{
		if (Weapon->GetWeaponMesh())
		{
			Weapon->GetWeaponMesh()->SetRelativeLocation(OriginalLocation);
		}
	}, 0.1f, false); 
}

void URaycastEffectManagerComponent::ImpactEffect(const FHitResult& HitResult) const
{
	if (RaycastWeaponData->WeaponEffects.ImpactEffect)
	{
		FVector ImpactLocation = HitResult.ImpactPoint;
		FRotator ImpactRotation = HitResult.ImpactNormal.Rotation();
	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RaycastWeaponData->WeaponEffects.ImpactEffect, ImpactLocation, ImpactRotation);
	}
}

void URaycastEffectManagerComponent::ApplyDecal(const FHitResult& HitResult) const
{
	UE_LOG(LogTemp, Warning, TEXT("ApplyDecal() called. Impact point: %s, Impact normal: %s"), *HitResult.ImpactPoint.ToString(), *HitResult.ImpactNormal.ToString());

	if (RaycastWeaponData->DecalSettings.DecalMaterial)
	{
		if (UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			RaycastWeaponData->DecalSettings.DecalMaterial,
			RaycastWeaponData->DecalSettings.DecalSize,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation(),
			RaycastWeaponData->DecalSettings.DecalLifeSpan
		))
		{
			UE_LOG(LogTemp, Warning, TEXT("Decal successfully spawned."));
			Decal->SetFadeOut(RaycastWeaponData->DecalSettings.DecalLifeSpan, 2.0);
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
