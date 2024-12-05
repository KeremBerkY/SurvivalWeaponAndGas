#pragma once

#include "CoreMinimal.h"


	UENUM(BlueprintType)
	enum class ECharacterState : uint8
	{
		Idle UMETA(DisplayName = "Idle"),
		Walking UMETA(DisplayName = "Walking"),
		Running UMETA(DisplayName = "Running"),
		Attacking UMETA(DisplayName = "Attacking"),
		Sprinting UMETA(DisplayName = "Sprinting"),
		Aiming UMETA(DisplayName = "Aiming"),
		AimingWhileMoving UMETA(DisplayName = "Aiming While Moving"),
		Crouching UMETA(DisplayNme = "Crouching")
	};


	UENUM(BlueprintType)
	enum class ECharacterWeaponStates : uint8
	{
		ECS_Unarmed UMETA(DisplayName = "Unarmed"),
		ECS_CurrentWeaponRaycast UMETA(DisplayName = "Current Weapon Raycast"),
		ECS_CurrentWeaponProjectile UMETA(DisplayName = "Current Weapon Projectile"),
		ECS_CurrentWeaponMelee UMETA(DisplayName = "Current Weapon Melee")
	};
	