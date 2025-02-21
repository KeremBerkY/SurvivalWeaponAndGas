// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "WeaponInventory.generated.h"

// TODO: Change class name with WeaponInventoryComponent

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponInventoryReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResetSlot, EWeaponCategory, WeaponCategory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateInventory, AWeaponBase*, WeaponToUpdate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponInventory();

	// DELEGATE
	FOnResetSlot OnResetSlot;
	FOnUpdateInventory OnUpdateInventory;
	FOnWeaponInventoryReady WeaponInventoryReady;

	FORCEINLINE UTexture2D* GetDefaultSlotTexture() const { return DefaultSlotTexture; }

	UFUNCTION()
	void SwapToBackWeapon(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategory DesiredCategory);
	void InitializeCategoryMap();
	void AddWeaponToSlot(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter);
	void RemoveFromSlot(AWeaponBase* WeaponToRemove);

	int32 GetSlotIndex(EWeaponCategory Category) const;

	bool HasWeaponInCategory(EWeaponCategory DesiredCategory) const;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
	TMap<int32, AWeaponBase*> WeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
	TMap<EWeaponCategory, int32> CategoryToSlotMap;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCharacterAnimInstance* CharacterAnimInstance;

private:
	
	// bool ValidateInputs(const ASurvivalCharacter* PlayerCharacter) const;
	void EquipBackWeapon(AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex);
	void MoveCurrentWeaponToBack(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex);
	void SwapWeapons(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex);
	void HandleDifferentCategorySwap(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeaponForDesiredCategory,ASurvivalCharacter* PlayerCharacter, int32 DesiredSlotIndex, EWeaponCategory DesiredCategory);

	void NotifyWeaponInventoryReady();
	
	FName WeaponSocket = "WeaponSocket";
	
	FName DefaultSocketName = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Weapon Inventory")
	UTexture2D* DefaultSlotTexture;
};
