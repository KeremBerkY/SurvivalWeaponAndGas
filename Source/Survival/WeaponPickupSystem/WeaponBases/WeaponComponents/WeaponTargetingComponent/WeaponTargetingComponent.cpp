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
// FVector UWeaponTargetingComponent::GetRandomBoneLocation(const ASurvivalEnemyCharacter* Enemy) const
// {
// 	if (!Enemy || !Enemy->GetMesh())
// 	{
// 		return FVector::ZeroVector;
// 	}
//
// 	const USkeletalMeshComponent* EnemyMesh = Enemy->GetMesh();
//
// 	for (int32 i = 0; i < TargetBoneNames.BoneNames.Num(); ++i)
// 	{
// 		const int32 RandomIndex = FMath::RandRange(0, TargetBoneNames.BoneNames.Num() - 1);
// 		const FName BoneToCheck = TargetBoneNames.BoneNames[RandomIndex];
//
// 		if (EnemyMesh->GetBoneIndex(BoneToCheck) != INDEX_NONE)
// 		{
// 			return EnemyMesh->GetBoneLocation(BoneToCheck);
// 		}
// 	}
//
// 	return FVector::ZeroVector;
// }

FVector UWeaponTargetingComponent::GetRandomBoneLocation(const ASurvivalEnemyCharacter* Enemy) const
{
	if (!Enemy || !Enemy->GetMesh() || !Enemy->GetMesh()->GetSkeletalMeshAsset())
	{
		return FVector::ZeroVector;
	}

	const USkeletalMeshComponent* EnemyMesh = Enemy->GetMesh();
	const FReferenceSkeleton& RefSkeleton = EnemyMesh->GetSkeletalMeshAsset()->GetRefSkeleton();

	// // We check whether the bone names defined in the filter list are present in the enemy skeleton.
	// TArray<FName> ValidBoneNames;
	// for (const FName& FilterBone : TargetBoneNames.BoneNames)
	// {
	// 	if (RefSkeleton.FindBoneIndex(FilterBone) != INDEX_NONE)
	// 	{
	// 		ValidBoneNames.Add(FilterBone);
	// 	}
	// }
	//
	// // Eğer filtre listesinde geçerli kemikler varsa, onlardan rastgele seçim yap.
	// if (ValidBoneNames.Num() > 0)
	// {
	// 	int32 RandomIndex = FMath::RandRange(0, ValidBoneNames.Num() - 1);
	// 	FName SelectedBone = ValidBoneNames[RandomIndex];
	// 	return EnemyMesh->GetBoneLocation(SelectedBone);
	// }

	// If no valid bones are found in the filter list, randomly select from all skeleton bones.
	const int32 NumBones = RefSkeleton.GetNum();
	if (NumBones > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, NumBones - 1);
		FName RandomBoneName = RefSkeleton.GetBoneName(RandomIndex);
		return EnemyMesh->GetBoneLocation(RandomBoneName);
	}
    
	return FVector::ZeroVector;
}
