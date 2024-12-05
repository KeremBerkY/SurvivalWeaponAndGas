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
		// Burada Unarmed mı diye kontrol etmemiz lazım. Karakterin elinde silah varsa update etsin weapontype'ı
	
		AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon == nullptr)
		{
			// PlayerCharacter->CharacterStates = ECharacterStates::ECS_Unarmed; // Bu zaten yorum satırıydı kafan karışmasın eski sistemde de yoktu!
			// PlayerCharacter->SetCharacterWeaponState(ECharacterWeaponStates::ECS_Unarmed);
			PlayerCharacter->GetCharacterWeaponComponent()->SetCharacterWeaponState(ECharacterWeaponStates::ECS_Unarmed);
			SetCurrentWeaponType(EWeaponType::EWT_Unarmed);
			UE_LOG(LogTemp, Error, TEXT("SetCurrentWeaponType(EWeaponType::EWT_Unarmed);"))
		}
		else
		{
			EWeaponType NewWeaponType = Weapon->GetWeaponType();
			// CurrentWeaponType = NewWeaponType; // Bu zaten yorum satırıydı kafan karışmasın eski sistemde de yoktu!
			SetCurrentWeaponType(NewWeaponType);
			UE_LOG(LogTemp, Warning, TEXT("Update Weapon Type: %d"), static_cast<uint8>(CurrentWeaponType));
			// PlayerCharacter->UpdateCharacterWeaponState(Weapon);
			PlayerCharacter->GetCharacterWeaponComponent()->UpdateWeaponState(Weapon);
		}
	}
}