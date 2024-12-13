// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "Components/CharacterWeaponComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ASurvivalCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (PlayerCharacter)
	{
		FVector PlayerCharacterVelocity = PlayerCharacter->GetVelocity();
		Speed = PlayerCharacterVelocity.Size();
		
		FTransform PlayerCharacterTransform = PlayerCharacter->GetTransform();
		FVector LocalDirection = PlayerCharacterTransform.InverseTransformVectorNoScale(PlayerCharacterVelocity);
		FRotator Rotation = LocalDirection.Rotation();
		Angle = Rotation.Yaw;
		
	}
}

void UCharacterAnimInstance::UpdateWeaponType(AWeaponBase* Weapon) // TODO: Buna bak nerelerde çağrılmış
{
	if (PlayerCharacter)
	{
		AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon == nullptr)
		{
			PlayerCharacter->GetCharacterWeaponComponent()->SetCharacterWeaponState(ECharacterWeaponStates::ECS_Unarmed);
			SetCurrentWeaponType(EWeaponType::Ewt_Unarmed);
			UE_LOG(LogTemp, Warning, TEXT("SetCurrentWeaponType(EWeaponType::EWT_Unarmed);"))
		}
		else
		{
			EWeaponType NewWeaponType = Weapon->GetWeaponType();
			SetCurrentWeaponType(NewWeaponType);
			UE_LOG(LogTemp, Warning, TEXT("Update Weapon Type: %d"), static_cast<uint8>(ActiveWeaponType));
			PlayerCharacter->GetCharacterWeaponComponent()->UpdateWeaponState(Weapon);
		}
	}
}
