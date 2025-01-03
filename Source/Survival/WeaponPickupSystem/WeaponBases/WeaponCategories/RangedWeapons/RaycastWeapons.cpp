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

	CalculateTargetPoint();
	
	UpdateHeat.Broadcast();
	DecreaseCurrentAmmo();
}

void ARaycastWeapons::CalculateTargetPoint()
{
	const UCharacterCameraComponent* Camera = GetOwningCharacter()->GetCharacterCameraComponent();
	const auto PlayerCharacter = GetOwningCharacter(); // TODO: sadece local kullanıyorsan Headerdan kaldır.
	
	if (!GetOwningCharacter() || !Camera || !PlayerCharacter) return;
	
	if (Camera->IsAiming())
	{
		RaycastMuzzleLocation = Camera->GetComponentLocation();
		RaycastMuzzleForward = Camera->GetForwardVector();
		RaycastTraceEnd = RaycastMuzzleLocation + (RaycastMuzzleForward * 10000.0f);
	}
	else if (PlayerCharacter->GetLockonComponent()->IsLocked())
	{
		RaycastMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
		RaycastMuzzleForward = GetMuzzleLocation()->GetForwardVector();
		if (GetOwningCharacter())
		{
			RaycastTraceEnd = WeaponTargetingComponent->CalculateTargetLocation(PlayerCharacter);
		}
	}
	else
	{
		RaycastMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
		RaycastMuzzleForward = GetMuzzleLocation()->GetForwardVector();
		RaycastTraceEnd = RaycastMuzzleLocation + (RaycastMuzzleForward * 10000.0f);
	}

}


void ARaycastWeapons::DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const
{
	// Raycast çizgisi
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	
	// Çarpma noktası üzerinde bir küre çiz
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 1.0f);
	
		// Çarpılan yerin adını logla
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

// DENEME
// if (AActor* TargetActor = PlayerCharacterPtr.Get()->GetLockonComponent()->GetCurrentTargetActor())
// {
// 	EnemyPtr = Cast<AEnemyBase>(TargetActor);
// 	if (EnemyPtr.IsValid())
// 	{
// 				
// 		if (auto EnemyMesh = EnemyPtr->GetMesh())
// 		{
// 			TArray<FName> TargetBones = {
// 				TEXT("Pelvis"),
// 				TEXT("loin_cloth_bk_r_02"),
// 				TEXT("loin_cloth_fr_l_01"),
// 				TEXT("big_pack_l"),
// 				TEXT("big_pack_r"),
// 				TEXT("belt"),
// 				TEXT("spine_02"),
// 				TEXT("spine_03"),
// 				TEXT("clavicle_l"),
// 				TEXT("lowerarm_l"),
// 				TEXT("upperarm_twist_02_l"),
// 				TEXT("upperarm_r"),
// 				TEXT("lowerarm_r"),
// 				TEXT("muscle_pec_r"),
// 				TEXT("muscle_pec_l"),
// 				TEXT("neck_03"),
// 				TEXT("head")
// 			};
// 					
// 			FName SelectedBone = NAME_None;
// 			for (int32 i = 0; i < TargetBones.Num(); ++i)
// 			{
// 				int32 RandomIndex = FMath::RandRange(0, TargetBones.Num() - 1);
// 				FName BoneToCheck = TargetBones[RandomIndex];
//
// 				// GetBoneIndex ile kemiğin var olup olmadığını kontrol et
// 				if (EnemyMesh->GetBoneIndex(BoneToCheck) != INDEX_NONE)
// 				{
// 					SelectedBone = BoneToCheck;
// 					break; // Geçerli bir kemik bulundu, döngüden çık
// 				}
// 			}
//
// 			// Eğer geçerli bir kemik bulunduysa hedefle
// 			if (SelectedBone != NAME_None)
// 			{
// 				FVector BoneLocation = EnemyMesh->GetBoneLocation(SelectedBone);
// 				RaycastTraceEnd = BoneLocation;
// 			}
// 			else
// 			{
// 				// Eğer geçerli bir kemik bulunamazsa alternatif bir hedefleme yap
// 				FVector ActorBoundsOrigin;
// 				FVector ActorBoundsExtent;
// 				TargetActor->GetActorBounds(true, ActorBoundsOrigin, ActorBoundsExtent);
//
// 				float RandomHeightOffset = FMath::RandRange(ActorBoundsExtent.Z * 0.5f, ActorBoundsExtent.Z);
// 				RaycastTraceEnd = TargetActor->GetActorLocation() + FVector(0, 0, RandomHeightOffset);
// 			}
// 					
// 		}
// 	}
// }