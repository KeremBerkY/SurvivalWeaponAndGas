// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTargetingComponent.generated.h"


class AEnemyBase;
class ASurvivalCharacter;

USTRUCT(BlueprintType)
struct FTargetBoneNames
{
	GENERATED_USTRUCT_BODY()

	FTargetBoneNames()
	{
		BoneNames = {
			TEXT("Pelvis"),
			TEXT("loin_cloth_bk_r_02"),
			TEXT("loin_cloth_fr_l_01"),
			TEXT("big_pack_l"),
			TEXT("big_pack_r"),
			TEXT("belt"),
			TEXT("spine_02"),
			TEXT("spine_03"),
			TEXT("clavicle_l"),
			TEXT("lowerarm_l"),
			TEXT("upperarm_twist_02_l"),
			TEXT("upperarm_r"),
			TEXT("lowerarm_r"),
			TEXT("muscle_pec_r"),
			TEXT("muscle_pec_l"),
			TEXT("neck_03"),
			TEXT("head")
		};
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<FName> BoneNames;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UWeaponTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponTargetingComponent();

	FTargetBoneNames TargetBoneNames;

	FVector CalculateTargetLocation(const ASurvivalCharacter* PlayerCharacter) const;

private:
	FVector GetRandomBoneLocation(const AEnemyBase* Enemy) const;
	
};
