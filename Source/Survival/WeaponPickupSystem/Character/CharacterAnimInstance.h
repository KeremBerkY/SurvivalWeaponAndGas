// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RaycastWeapons.h"
#include "CharacterAnimInstance.generated.h"

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

	FORCEINLINE void SetCurrentWeaponType(EWeaponType NewWeaponType) { ActiveWeaponType = NewWeaponType; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UpdateWeaponType(AWeaponBase* Weapon);
	
	UPROPERTY(BlueprintReadOnly)
	ASurvivalCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	// EWeaponType CurrentWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType ActiveWeaponType;
protected:
	

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Angle;
	
};
