// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInventoryWidget.generated.h"

class UImage;
class UWeaponInventory;
class AWeaponBase;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCallback(UWeaponInventory* WeaponInventory);
	
	UFUNCTION()
	void ResetSlotToDefault(EWeaponCategories WeaponCategory);
	
	UFUNCTION()
	void UpdateInventory(AWeaponBase* Weapon);

	
protected:
	virtual void NativeConstruct() override;

private:
	// Silah kategorisini slot indeksine eşleştiren yardımcı fonksiyon
	int32 GetSlotIndexForCategory(EWeaponCategories WeaponCategory) const;


	// UPROPERTY(meta = (BindWidget))
	// UImage* DefaultSlotImage;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UTexture2D* DefaultSlotTexture;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Slot1Image;

	UPROPERTY(meta = (BindWidget))
	UImage* Slot2Image;

	UPROPERTY(meta = (BindWidget))
	UImage* Slot3Image;


	TArray<UImage*> SlotImages;
};
