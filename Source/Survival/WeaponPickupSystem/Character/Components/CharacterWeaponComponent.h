// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "CharacterWeaponComponent.generated.h"

class UWeaponAttachmentManager;
class ASurvivalCharacter;
class AWeaponBase;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UCharacterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterWeaponComponent();
	
						/* Getters / Setters */
	// Weapon State
	FORCEINLINE ECharacterWeaponStates GetWeaponState() const { return CharacterWeaponState; }
	FORCEINLINE void SetCharacterWeaponState(ECharacterWeaponStates NewWeaponState) { CharacterWeaponState = NewWeaponState; }
	// CurrentWeapon
	FORCEINLINE AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* NewWeapon) { CurrentWeapon = NewWeapon; }
	FORCEINLINE UWeaponAttachmentManager* GetWeaponAttachmentManager() const { return WeaponAttachmentManager; }
	FORCEINLINE ASurvivalCharacter* GetOwningCharacter() const { return Cast<ASurvivalCharacter>(GetOwner()); }


	//DENEM:
	void UpdateCurrentWeaponUI() const;
	void RemoveCurrentWeaponUI() const;
	void BindUICallbacks(AWeaponBase* WeaponBase, ASurvivalCharacter* PlayerCharacter) const;
	
	UFUNCTION()
	void UpdateWeaponState(AWeaponBase* EquippedWeapon);
	
	void DropWeapon(const ASurvivalCharacter* PlayerCharacter, AWeaponBase* WeaponToDrop = nullptr);

	// Calling from Abilities
	bool CanSwitchWeapon() const;
	void UpdateLastSwitchTime();
	
	void SpawnAndAddWeapon(const TSubclassOf<AWeaponBase>& WeaponInstance, ASurvivalCharacter* PlayerCharacter);
	void EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent = true); 
	void AddWeaponToCharacter(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter);

	// void CurrentWeaponUICheck();
	// void CurrentWeaponUIRemove();

private:

	UPROPERTY()
	UWeaponAttachmentManager* WeaponAttachmentManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	ECharacterWeaponStates CharacterWeaponState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Switching", meta = (AllowPrivateAccess = "true"))
	float WeaponSwitchCooldown = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Switching")
	float LastSwitchTime;

	UPROPERTY(EditAnywhere, Category  = "Weapon")
	float DropDistance = 200.f;
};
