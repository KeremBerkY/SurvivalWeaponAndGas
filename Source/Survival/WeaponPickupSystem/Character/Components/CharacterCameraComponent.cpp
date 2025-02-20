// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterStateComponent.h"

UCharacterCameraComponent::UCharacterCameraComponent()
{
	bAiming = false;
	CameraDefaultFOV = 0.f;
	CameraZoomedFOV = 40.f;
	DefaultSocketOffset = FVector(0.f, 65.f, 35.f);
	AimSocketOffset = FVector(0.f, 65.f, 60.f);
	CameraCurrentFOV = 0.f;
	ZoomInterpSpeed = 25.f;
}

void UCharacterCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacterPtr = MakeWeakObjectPtr(Cast<ASurvivalCharacter>(GetOwner()));
	if (PlayerCharacterPtr.IsValid())
	{
		CameraBoomPtr = MakeWeakObjectPtr(PlayerCharacterPtr->GetCameraBoom());
		CameraDefaultFOV = FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
}

void UCharacterCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CameraInterpZoom(DeltaTime);

}

void UCharacterCameraComponent::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed
		);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed
		);
	}
	SetFieldOfView(CameraCurrentFOV);
}

void UCharacterCameraComponent::AimingButtonPressed()
{
	bAiming = true;
	if (CameraBoomPtr.IsValid())
	{
		CameraBoomPtr->SocketOffset = AimSocketOffset;
	}
	OnAiming.Broadcast(PlayerCharacterPtr.Get());
}

void UCharacterCameraComponent::AimingButtonReleased()
{
	bAiming = false;
	if (CameraBoomPtr.IsValid())
	{
		CameraBoomPtr->SocketOffset = DefaultSocketOffset;
	}
	OnAiming.Broadcast(PlayerCharacterPtr.Get());
}

