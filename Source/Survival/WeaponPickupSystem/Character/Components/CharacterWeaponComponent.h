// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "CharacterWeaponComponent.generated.h"

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
	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	/** Silah durumunu güncelle */
	UFUNCTION()
	void UpdateWeaponState(AWeaponBase* EquippedWeapon);

	/** Mevcut silahı düşür */
	void DropWeapon(ASurvivalCharacter* PlayerCharacter); // Parametre olarak PlayerCharacter'i ver. Diğer bütün sınıflar bını çağıracak Drop işleminde.EXP; WeaponInventory
protected:

	virtual void BeginPlay() override;

private:
	/** Mevcut silah durumu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	ECharacterWeaponStates CharacterWeaponState;

	/** Sahip olunan silah referansı */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* CurrentWeapon;

};
