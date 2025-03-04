// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeapons.h"

#include "GameFramework/SpringArmComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/SingleShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponAnimationsComponent/WeaponAnimationsComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/RaycastEffectManagerComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponTargetingComponent/WeaponTargetingComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"


ARaycastWeapons::ARaycastWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EffectManagerComponent = CreateDefaultSubobject<URaycastEffectManagerComponent>(TEXT("WeaponEffectManagerComponent"));
	WeaponAnimationsComponent = CreateDefaultSubobject<UWeaponAnimationsComponent>(TEXT("WeaponAnimationsComponent"));
	HeatComponent = CreateDefaultSubobject<UHeatComponent>(TEXT("HeatComponent"));
	RaycastWeaponUIHandler = CreateDefaultSubobject<URaycastWeaponUIHandler>(TEXT("RaycastWeaponUIHandler"));
	
	SingleShotModeComponent = CreateDefaultSubobject<USingleShotModeComponent>(TEXT("SingleShotModeComponent"));
	WeaponTargetingComponent = CreateDefaultSubobject<UWeaponTargetingComponent>(TEXT("WeaponTargetingComponent"));
	
}

void ARaycastWeapons::BeginPlay()
{
	Super::BeginPlay();

	if (URaycastWeaponData* WeaponData = Cast<URaycastWeaponData>(GetWeaponDataAsset()))
	{
		RaycastWeaponData = WeaponData;
		OnInitializedComponents.Broadcast();
	}

	SetCurrentAmmo(RaycastWeaponData->WeaponAmmoAttributes.CurrentAmmo);
	
	AddFireModes();
	
}

void ARaycastWeapons::AddFireModes()
{
	FireModeComponents.Add(SingleShotModeComponent);
}

void ARaycastWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARaycastWeapons::Attack()
{
	Super::Attack();

	if (GetCurrentAmmo() <= 0 || !CanFire() || HeatComponent->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || HeatComponent->IsOverHeated()) return;
	
	if (CurrentFireModeComponent && CanFire())
	{
		CurrentFireModeComponent->Fire();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentFireModeComponent is valid: %s"), *CurrentFireModeComponent->GetName());
	}
}

void ARaycastWeapons::EndAttack()
{
	Super::EndAttack();

	if (CurrentFireModeComponent)
	{
		CurrentFireModeComponent->EndFire();
	}
	
}

void ARaycastWeapons::PerformFire()
{
	Super::PerformFire();

	if (GetCurrentAmmo() <= 0 || HeatComponent->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || HeatComponent->IsOverHeated()) return;
	
	// GetOwningCharacter()->GetCharacterAbilitySystemComponent()->TryActivateAbilityByClass(this->GetRaycastWeaponDataAsset()->FireAbility);

	CalculateTargetPoint();
	
	DecreaseCurrentAmmo();
	
	UpdateHeat.Broadcast();

	OnAmmoChange.Broadcast(GetCurrentAmmo(), GetTotalAmmo());
}

void ARaycastWeapons::CalculateTargetPoint()
{
	const UCharacterCameraComponent* Camera = GetOwningCharacter()->GetCharacterCameraComponent();
	const auto PlayerCharacter = GetOwningCharacter(); // TODO: sadece local kullanıyorsan Headerdan kaldır.
	
	if (!GetOwningCharacter() || !Camera || !PlayerCharacter) return;

	const auto RecoilSettings = GetRaycastWeaponDataAsset()->RecoilSettings;

	const FVector Velocity = PlayerCharacter->GetVelocity();
	const bool bIsMoving = !Velocity.IsNearlyZero();

	float VerticalRecoil = 0.0f;
	float HorizontalRecoil = 0.0f;

	if (bIsMoving)
	{
		VerticalRecoil = FMath::FRandRange(-RecoilSettings.PitchMultiplier, RecoilSettings.PitchMultiplier);
		HorizontalRecoil = FMath::FRandRange(-RecoilSettings.YawMultiplier, RecoilSettings.YawMultiplier);
	}
	
	FVector AdjustedForwardVector = Camera->GetForwardVector();
	FRotator RecoilRotator = FRotator(-VerticalRecoil, HorizontalRecoil, 0.0f);
	AdjustedForwardVector = RecoilRotator.RotateVector(AdjustedForwardVector);
	
	if (Camera->IsAiming())
	{
		RaycastMuzzleLocation = Camera->GetComponentLocation();
		RaycastMuzzleForward = AdjustedForwardVector;
	}
	else if (PlayerCharacter->GetLockonComponent()->IsLocked())
	{
		RaycastMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
		if (GetOwningCharacter())
		{
			RaycastMuzzleForward = AdjustedForwardVector;
			RaycastTraceEnd = WeaponTargetingComponent->CalculateTargetLocation(PlayerCharacter);
			return;
		}
	}
	else
	{
		RaycastMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
		RaycastMuzzleForward = AdjustedForwardVector;
	}

	RaycastTraceEnd = RaycastMuzzleLocation + (RaycastMuzzleForward * 10000.0f);

}


void ARaycastWeapons::DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 1.0f);
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Impact Point: %s"), *HitActor->GetName(), *HitResult.ImpactPoint.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No actor hit. Impact Point: %s"), *HitResult.ImpactPoint.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast did not hit any surface."));
	}
}
