// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "CharacterCameraComponent.generated.h"

class ASurvivalCharacter;

USTRUCT(BlueprintType)
struct FCameraDistances
{
	GENERATED_USTRUCT_BODY()

	FCameraDistances() :
			DefaultDistance(200.0f),
			SprintDistance(350.0f), // Sword fight da Sprint Distance ile aynnı olsun.
			AimDistance(200.0f), // Değiştir
			TransitionSpeed(5.0f) 
	{}

	UPROPERTY(EditAnywhere, Category = "Camera")
	float DefaultDistance;

	/** Sprint camera distance */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float SprintDistance;

	/** Aim camera distance */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float AimDistance;

	/** Smooth transition speed */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float TransitionSpeed;
};


class USpringArmComponent;

UCLASS()
class SURVIVAL_API UCharacterCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	// void SetupCharacterStateListener(ASurvivalCharacter* PlayerCharacter);

protected:

private:
	// void HandleCharacterStateChanged(ECharacterState NewState);
	
	
};
