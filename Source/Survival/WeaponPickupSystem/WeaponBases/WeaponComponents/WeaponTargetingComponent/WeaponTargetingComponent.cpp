// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTargetingComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalEnemyCharacter.h"
#include "Survival/WeaponPickupSystem/Enemy/EnemyBase.h"


UWeaponTargetingComponent::UWeaponTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

FVector UWeaponTargetingComponent::CalculateTargetLocation(const ASurvivalCharacter* PlayerCharacter) const // TODO: GetCurrentTargetActor() zaten enemy. Castleri kaldır!!
{
	// if (const AEnemyBase* Enemy = PlayerCharacter->GetLockonComponent()->GetCurrentTargetActor())
	if (const ASurvivalEnemyCharacter* Enemy = PlayerCharacter->GetLockonComponent()->GetCurrentTargetActor())
	{
		const FVector BoneLocation = GetRandomBoneLocation(Enemy);
		if (!BoneLocation.IsZero())
		{
			return BoneLocation;
		}
		

		FVector ActorBoundsOrigin;
		FVector ActorBoundsExtent;
		Enemy->GetActorBounds(true, ActorBoundsOrigin, ActorBoundsExtent);

		const float RandomHeightOffset = FMath::RandRange(ActorBoundsExtent.Z * 0.5f, ActorBoundsExtent.Z);
		return Enemy->GetActorLocation() + FVector(0, 0, RandomHeightOffset);
	}
	
	return FVector::ZeroVector;
}

// FVector UWeaponTargetingComponent::GetRandomBoneLocation(const AEnemyBase* Enemy) const
FVector UWeaponTargetingComponent::GetRandomBoneLocation(const ASurvivalEnemyCharacter* Enemy) const
{
	if (!Enemy || !Enemy->GetMesh())
	{
		return FVector::ZeroVector;
	}

	const USkeletalMeshComponent* EnemyMesh = Enemy->GetMesh();

	for (int32 i = 0; i < TargetBoneNames.BoneNames.Num(); ++i)
	{
		const int32 RandomIndex = FMath::RandRange(0, TargetBoneNames.BoneNames.Num() - 1);
		const FName BoneToCheck = TargetBoneNames.BoneNames[RandomIndex];

		if (EnemyMesh->GetBoneIndex(BoneToCheck) != INDEX_NONE)
		{
			return EnemyMesh->GetBoneLocation(BoneToCheck);
		}
	}

	return FVector::ZeroVector;
}
