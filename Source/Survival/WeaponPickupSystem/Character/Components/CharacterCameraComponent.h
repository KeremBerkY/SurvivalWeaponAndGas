// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "CharacterCameraComponent.generated.h"

class ASurvivalCharacter;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAiming, ASurvivalCharacter*, Character);

UCLASS()
class SURVIVAL_API UCharacterCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UCharacterCameraComponent();

	/* Delegates */
	FOnAiming OnAiming;

	FORCEINLINE bool IsAiming() const { return bAiming;  }
	FORCEINLINE FVector GetAimSocketOffset() const { return AimSocketOffset; }
	FORCEINLINE FVector GetDefaultSocketOffset() const { return DefaultSocketOffset; }
	FORCEINLINE TWeakObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoomPtr; }

	void AimingButtonPressed();
	void AimingButtonReleased();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CameraInterpZoom(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming;
	
	float CameraDefaultFOV;
	
	float CameraZoomedFOV;

	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

private:
	UPROPERTY()
	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;

	UPROPERTY()
	TWeakObjectPtr<USpringArmComponent> CameraBoomPtr;

	FVector DefaultSocketOffset;
	FVector AimSocketOffset;
	
};
