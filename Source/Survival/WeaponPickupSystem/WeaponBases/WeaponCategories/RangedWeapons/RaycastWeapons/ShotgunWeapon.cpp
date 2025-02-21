// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/BurstShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/FireModeBaseComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

AShotgunWeapon::AShotgunWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BurstShotModeComponent = CreateDefaultSubobject<UBurstShotModeComponent>(TEXT("BurstShotModeComponent"));
}
void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgunWeapon::AddFireModes()
{
	Super::AddFireModes();

	FireModeComponents.Add(BurstShotModeComponent);
	
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

void AShotgunWeapon::PerformFire() // TODO: aim ile tam ortaya ateş edilmiyor, kitlenince enemynin ortasına ateş edilmiyor ve enemy üzerinde debugsphere çalışmıyor.
{
	Super::PerformFire();

	if (GetHeatComponent()->GetCurrentHeat() >= GetRaycastWeaponDataAsset()->FiringHeatSettings.MaxHeatCapacity || GetHeatComponent()->IsOverHeated() ) { return; }

	FVector LocalMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
	// FVector MuzzleForward = GetMuzzleLocation()->GetForwardVector();
	
	// Saçma sayısı ve yayılma açısı
	int32 PelletCount = 10; // Saçma sayısı
	float SpreadAngle = 10.0f; // Maksimum yayılma açısı (derece)
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	// OnFirePlayMontage.Broadcast();
	
	for (int32 i = 0; i < PelletCount; i++)
	{
		FVector RandomSpread = FMath::VRandCone(RaycastMuzzleForward, FMath::DegreesToRadians(SpreadAngle));
		// FVector TraceEnd = RaycastMuzzleLocation + (RandomSpread * 5000.0f);
		//
		// if (PlayerCharacterPtr.Get()->GetLockonComponent()->IsLocked())
		// {
		// 	if (const UCharacterCameraComponent* Camera = GetOwningCharacter()->GetCharacterCameraComponent())
		// 	{
		// 		RaycastMuzzleLocation = Camera->GetComponentLocation();
		// 	}
		// }
		
		FVector TraceEnd = LocalMuzzleLocation + (RandomSpread * 5000.0f);

		OnFireMade.Broadcast();
		// OnFirePlayMontage.Broadcast();
		
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(
		   HitResult,
		   LocalMuzzleLocation,
		   TraceEnd,
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

		DrawDebugLine(GetWorld(), LocalMuzzleLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);

	}
}




