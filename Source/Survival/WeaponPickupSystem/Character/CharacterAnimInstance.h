// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "CharacterAnimInstance.generated.h"

enum class EWeaponTypes : uint8;
class AWeaponBase;
class UCharacterMovementComponent;
class ASurvivalCharacter;
/**
Optimize Edilebilir Yönler:
Silah türü ve animasyon geçiş süreleri gibi verileri daha esnek hale getirmek için DataTable veya başka bir yapı kullanabilirsin.
 */
UCLASS()
class SURVIVAL_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	FORCEINLINE void SetCurrentWeaponType(EWeaponTypes NewWeaponType) { ActiveWeaponType = NewWeaponType; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UpdateWeaponType(AWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void UpdateCharacterRotation();

	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponTypes ActiveWeaponType;
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Angle;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rotation")
	float AimPitch;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation")
	float AimPitchForNeck;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation")
	float AimYaw;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation")
	float AimRoll;
	
};
