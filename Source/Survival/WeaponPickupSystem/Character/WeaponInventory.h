// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "WeaponInventory.generated.h"

// TODO: Change class name with WeaponInventoryComponent

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponInventory();

	FORCEINLINE UTexture2D* GetDefaultSlotTexture() const { return DefaultSlotTexture; }
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SwapToBackWeapon(AWeaponBase* CurrentWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategories DesiredCategory);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
	TMap<int32, AWeaponBase*> WeaponSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
	TMap<EWeaponCategories, int32> CategoryToSlotMap;

	void InitializeCategoryMap();
	void AddWeaponToSlot(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter);
	void AttachWeaponToSocket(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter, FName OverrideSocketName);
	void RemoveFromSlot(AWeaponBase* WeaponToRemove);

	int32 GetSlotIndex(EWeaponCategories Category) const;
	void EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCharacterAnimInstance* CharacterAnimInstance;

private:
	void UpdateHUDForWeaponSwap(AWeaponBase* CurrentWeapon, AWeaponBase* BackWeapon, ASurvivalCharacter* PlayerCharacter, EWeaponCategories DesiredCategory);
	
	FName WeaponSocket = "WeaponSocket";
	
	FName NameOfTheWeaponAccordingToType = NAME_None; // NAME_None için de bir global değişken yaz daha anlaşılır olsun.

	UPROPERTY(EditAnywhere, Category = "Weapon Inventory")
	UTexture2D* DefaultSlotTexture;
};
