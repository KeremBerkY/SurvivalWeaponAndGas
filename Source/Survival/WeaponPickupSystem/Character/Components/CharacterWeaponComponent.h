// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "CharacterWeaponComponent.generated.h"

class UWeaponAttachmentManager;
class ASurvivalCharacter;
class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUpdated, AWeaponBase*, Weapon); 


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UCharacterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterWeaponComponent();

	/* DELEGATES */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWeaponUpdated OnWeaponUpdated; 
	

						/* Getters / Setters */
	// Weapon State
	FORCEINLINE ECharacterWeaponStates GetWeaponState() const { return CharacterWeaponState; }
	FORCEINLINE void SetCharacterWeaponState(ECharacterWeaponStates NewWeaponState) { CharacterWeaponState = NewWeaponState; }
	// CurrentWeapon
	FORCEINLINE AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* NewWeapon) { CurrentWeapon = NewWeapon; }
	FORCEINLINE UWeaponAttachmentManager* GetWeaponAttachmentManager() const { return WeaponAttachmentManager; }
	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void UpdateWeaponState(AWeaponBase* EquippedWeapon);
	
	void DropWeapon(ASurvivalCharacter* PlayerCharacter, AWeaponBase* WeaponToDrop = nullptr); // Parametre olarak PlayerCharacter'i ver. Diğer bütün sınıflar bını çağıracak Drop işleminde.EXP; WeaponInventory

	// Calling from Abilities
	bool CanSwitchWeapon() const;
	void UpdateLastSwitchTime();

	// YAZILACAKLAR:
	void AddWeaponToCharacter(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter);

	// DENEMELER:
	// void EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, bool bFromInventory, FName CustomSocketName); // 2
	void EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent = true); // 3

	// DENEME:
	// void EquipWeapon(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter);
	// // void SetWeaponOwner(ASurvivalCharacter* NewOwner); TODO: Bunu kaldırabiliriz test et WeaponBase de
	void AddWeapon(const TSubclassOf<AWeaponBase>& WeaponInstance, ASurvivalCharacter* PlayerCharacter);
protected:

	virtual void BeginPlay() override;


private:

	UPROPERTY()
	UWeaponAttachmentManager* WeaponAttachmentManager;
	
	/** Mevcut silah durumu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	ECharacterWeaponStates CharacterWeaponState;

	/** Sahip olunan silah referansı */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Switching", meta = (AllowPrivateAccess = "true"))
	float WeaponSwitchCooldown = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Switching")
	float LastSwitchTime;

	UPROPERTY(EditAnywhere, Category  = "Weapon")
	float DropDistance = 200.f;
};
