// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "Components/CharacterCameraComponent.h"
#include "Components/CharacterWeaponComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacterPtr = MakeWeakObjectPtr(Cast<ASurvivalCharacter>(TryGetPawnOwner()));
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (PlayerCharacterPtr.IsValid())
	{
		const FVector PlayerCharacterVelocity = PlayerCharacterPtr->GetVelocity();
		Speed = PlayerCharacterVelocity.Size();

		const FTransform PlayerCharacterTransform = PlayerCharacterPtr->GetTransform();
		const FVector LocalDirection = PlayerCharacterTransform.InverseTransformVectorNoScale(PlayerCharacterVelocity);
		const FRotator Rotation = LocalDirection.Rotation();
		Angle = Rotation.Yaw;
	}
	
	if (PlayerCharacterPtr.IsValid() && 
		PlayerCharacterPtr->GetCharacterCameraComponent() && 
		PlayerCharacterPtr->GetCharacterCameraComponent()->IsAiming())
	{
		UpdateCharacterRotation();
	}
	else
	{
		AimPitch = 0.f;
		AimYaw = 0.f;
		AimRoll = 0.f;
		AimPitchForNeck = 0.f;
	}
}

void UCharacterAnimInstance::UpdateWeaponType(AWeaponBase* Weapon) // TODO: Buna bak nerelerde çağrılmış
{
	if (PlayerCharacterPtr.IsValid())
	{
		AWeaponBase* CurrentWeapon = PlayerCharacterPtr->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon == nullptr)
		{
			PlayerCharacterPtr->GetCharacterWeaponComponent()->SetCharacterWeaponState(ECharacterWeaponStates::ECS_Unarmed);
			SetCurrentWeaponType(EWeaponTypes::Ewt_Unarmed);
			UE_LOG(LogTemp, Warning, TEXT("SetCurrentWeaponType(EWeaponType::EWT_Unarmed);"))
		}
		else
		{
			EWeaponTypes NewWeaponType = Weapon->GetWeaponDataAsset()->WeaponAttributes.WeaponTypes;
			SetCurrentWeaponType(NewWeaponType);
			UE_LOG(LogTemp, Warning, TEXT("Update Weapon Type: %d"), static_cast<uint8>(ActiveWeaponType));
			PlayerCharacterPtr->GetCharacterWeaponComponent()->UpdateWeaponState(Weapon);
		}
	}
}

void UCharacterAnimInstance::UpdateCharacterRotation()
{
	const AController* Controller = PlayerCharacterPtr->GetController();
	if (!Controller) return;
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	
	const FRotator NewRotation = FRotator(0.0f, ControlRotation.Yaw, 0.0f);
	PlayerCharacterPtr->SetActorRotation(NewRotation);

	const float PitchValue = ControlRotation.Pitch;
	const float NormalizedPitch = FRotator::NormalizeAxis(PitchValue);

	constexpr float MinPitch = -5.0f;
	constexpr float MaxPitch = 15.0f;
	const float ClampedPitch = FMath::Clamp(NormalizedPitch , MinPitch, MaxPitch);
	
	AimPitch = FMath::FInterpTo(AimPitch, -NormalizedPitch, GetWorld()->GetDeltaSeconds(), 7.f);
	
	AimPitchForNeck = FMath::FInterpTo(AimPitchForNeck, -ClampedPitch, GetWorld()->GetDeltaSeconds(), 8.5f);

}
